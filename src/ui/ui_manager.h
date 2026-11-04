// Owns the active displays and inputs and keeps them in sync with the application state.
#pragma once

#include <stdint.h>

#include "ui/display.h"
#include "ui/input.h"
#include "ui/view_model.h"

class UiManager {
 public:
  static constexpr uint8_t kMaxDevices = 4;

  // Register devices before begin(). Returns false if the list is full.
  bool addDisplay(IDisplay* display);
  bool addInput(IInput* input);

  // Initialises all devices (dropping any that fail) and subscribes to controller state changes.
  void begin();

  // Shows a full-screen message and pauses normal rendering until showHome().
  void showStatus(const StatusScreen& status);
  void showMessage(const char* title, StatusTone tone = StatusTone::Info, const char* line = nullptr);
  void showProgress(const char* title, uint8_t percent);  // Same title keeps the same progress screen
  void showHome();

  // Request a render on the next loop() because application state changed.
  void markDirty();

  // Services devices, applies input events through the controller and renders when needed.
  void loop();

 private:
  void render();
  void handle(const InputEvent& event);

  IDisplay* displays_[kMaxDevices] = {};
  uint8_t displayCount_ = 0;
  IInput* inputs_[kMaxDevices] = {};
  uint8_t inputCount_ = 0;

  ViewModel vm_;
  uint32_t revision_ = 0;
  uint32_t lastRenderMs_ = 0;
  bool dirty_ = true;
  bool holdStatus_ = false;
};

extern UiManager ui;
