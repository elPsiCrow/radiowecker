#include "ui/ui_manager.h"

#include <Arduino.h>

#include "app/app_state.h"
#include "app/controller.h"
#include "app/log.h"
#include "app/view_model_builder.h"
#include "ui/i18n.h"

UiManager ui;

namespace {

constexpr uint32_t kClockRefreshMs = 1000;
constexpr uint8_t kMaxEventsPerPoll = 8;  // Bounds time spent on a flooded input

void onControllerChange(StateChange) { ui.markDirty(); }

template <typename T>
void removeAt(T* (&list)[UiManager::kMaxDevices], uint8_t& count, uint8_t index) {
  for (uint8_t i = index; i + 1 < count; i++) list[i] = list[i + 1];
  list[--count] = nullptr;
}

}  // namespace

bool UiManager::addDisplay(IDisplay* display) {
  if (displayCount_ >= kMaxDevices) return false;
  displays_[displayCount_++] = display;
  return true;
}

bool UiManager::addInput(IInput* input) {
  if (inputCount_ >= kMaxDevices) return false;
  inputs_[inputCount_++] = input;
  return true;
}

void UiManager::begin() {
  for (uint8_t i = 0; i < displayCount_;) {
    if (displays_[i]->begin()) {
      i++;
    } else {
      RADIO_SERIAL(Serial.printf("Display %s failed to start, dropped\n", displays_[i]->name()));
      removeAt(displays_, displayCount_, i);
    }
  }
  for (uint8_t i = 0; i < inputCount_;) {
    if (inputs_[i]->begin()) {
      i++;
    } else {
      RADIO_SERIAL(Serial.printf("Input %s failed to start, dropped\n", inputs_[i]->name()));
      removeAt(inputs_, inputCount_, i);
    }
  }
  addStateChangeListener(onControllerChange);
}

void UiManager::showStatus(const StatusScreen& status) {
  holdStatus_ = true;
  for (uint8_t i = 0; i < displayCount_; i++) displays_[i]->showStatus(status);
}

void UiManager::showMessage(const char* title, StatusTone tone, const char* line) {
  StatusScreen status;
  status.tone = tone;
  status.title = title;
  if (line) {
    status.lines[0] = line;
    status.lineCount = 1;
  }
  showStatus(status);
}

void UiManager::showProgress(const char* title, uint8_t percent) {
  StatusScreen status;
  status.title = title;
  status.progressPercent = (int8_t)(percent > 100 ? 100 : percent);
  showStatus(status);
}

void UiManager::showHome() {
  holdStatus_ = false;
  markDirty();
}

void UiManager::markDirty() { dirty_ = true; }

void UiManager::loop() {
  for (uint8_t i = 0; i < displayCount_; i++) displays_[i]->loop();

  for (uint8_t i = 0; i < inputCount_; i++) {
    InputEvent event;
    for (uint8_t n = 0; n < kMaxEventsPerPoll && inputs_[i]->poll(event); n++) handle(event);
  }

  if (i18n_take_ui_dirty()) {
    for (uint8_t i = 0; i < displayCount_; i++) displays_[i]->onLanguageChanged();
    markDirty();
  }

  if (newTitle) {
    newTitle = false;
    markDirty();
  }

  if (holdStatus_) return;

  const uint32_t now = millis();
  if (dirty_ || now - lastRenderMs_ >= kClockRefreshMs) render();
}

void UiManager::render() {
  if (dirty_) revision_++;
  buildViewModel(vm_);
  vm_.revision = revision_;
  for (uint8_t i = 0; i < displayCount_; i++) displays_[i]->render(vm_);
  dirty_ = false;
  lastRenderMs_ = millis();
}

void UiManager::handle(const InputEvent& event) {
  switch (event.type) {
    case InputEvent::Type::PlayPause:
      playbackToggle();
      break;
    case InputEvent::Type::Stop:
      alarmStop();
      break;
    case InputEvent::Type::Snooze:
      if (alarmActionsVisible) {
        alarmSnooze();
      } else {
        sleepTimerStart();
      }
      break;
    case InputEvent::Type::NextStation:
      stationStep(1);
      break;
    case InputEvent::Type::PrevStation:
      stationStep(-1);
      break;
    case InputEvent::Type::VolumeDelta:
      adjustVolumeBy(event.value);
      break;
    case InputEvent::Type::VolumeSet:
      adjustVolume((uint8_t)constrain((int)event.value, 0, 100));
      break;
    case InputEvent::Type::Refresh:
      for (uint8_t i = 0; i < displayCount_; i++) displays_[i]->redraw();
      markDirty();
      break;
  }
}
