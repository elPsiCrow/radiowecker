#include "drivers/display/lvgl_ili9341/lvgl_display.h"

#include <Adafruit_ILI9341.h>  // RGB565 color constants
#include <string.h>

#include "app/app_state.h"
#include "board_config.h"
#include "drivers/display/lvgl_ili9341/tft_display.h"

namespace {

constexpr uint16_t kScreenWidth = 320;
constexpr uint16_t kScreenHeight = 240;

// The message screen has five label slots at these y positions (see displayMessage()).
constexpr uint16_t kLineY[] = {50, 100, 150, 200};

uint16_t toneColor(StatusTone tone) {
  switch (tone) {
    case StatusTone::Ok:
      return ILI9341_GREEN;
    case StatusTone::Error:
      return ILI9341_RED;
    case StatusTone::Info:
    default:
      return ILI9341_YELLOW;
  }
}

}  // namespace

DisplayCapabilities LvglIli9341Display::capabilities() const {
  DisplayCapabilities caps;
  caps.graphical = true;
  caps.color = true;
  caps.touch = true;
  caps.width = kScreenWidth;
  caps.height = kScreenHeight;
  return caps;
}

bool LvglIli9341Display::begin() {
  setup_display();
  return true;
}

void LvglIli9341Display::showStatus(const StatusScreen& status) {
  statusShown_ = true;

  if (status.progressPercent >= 0) {
    // Firmware update: dedicated screen with a progress bar.
    if (otaTitle_ != status.title) {
      setBGLight(100);
      ota_ui_begin(status.title);
      otaTitle_ = status.title;
    }
    showProgress((uint32_t)status.progressPercent);
    return;
  }
  otaTitle_ = nullptr;

  displayClear();
  displayMessage(5, 10, 310, 30, status.title, ALIGNCENTER, true, toneColor(status.tone), ILI9341_BLACK, 1);
  for (uint8_t i = 0; i < status.lineCount && i < StatusScreen::kMaxLines; i++) {
    displayMessage(5, kLineY[i], 310, 30, status.lines[i], ALIGNCENTER, true, ILI9341_WHITE, ILI9341_BLACK, 1);
  }
}

void LvglIli9341Display::render(const ViewModel& vm) {
  if (statusShown_) {
    // Returning from a status screen: rebuild the start page and treat everything as changed.
    statusShown_ = false;
    otaTitle_ = nullptr;
    showStartPage();
    last_ = vm;
    haveLast_ = true;
    return;
  }

  const bool first = !haveLast_;

  if (first || strcmp(vm.streamTitle, last_.streamTitle) != 0 || vm.playback != last_.playback) showTitle();

  if (first || vm.playback != last_.playback || vm.alarm.ringing != last_.alarm.ringing ||
      vm.alarm.snoozed != last_.alarm.snoozed || vm.sleepTimerActive != last_.sleepTimerActive ||
      strcmp(vm.stationName, last_.stationName) != 0) {
    showRadio();
  }

  if (first || vm.alarm.armed != last_.alarm.armed || vm.alarm.weekday != last_.alarm.weekday ||
      vm.alarm.minutes != last_.alarm.minutes || vm.network.connected != last_.network.connected ||
      vm.sleepTimerActive != last_.sleepTimerActive) {
    showNextAlarm();
  }

  if (vm.weather.valid && (first || vm.weather.revision != last_.weather.revision)) {
    OpenMeteoTemps t;
    t.current_c = vm.weather.currentC;
    t.feels_c = vm.weather.feelsC;
    t.today_min = vm.weather.todayMinC;
    t.today_max = vm.weather.todayMaxC;
    t.tomorrow_min = vm.weather.tomorrowMinC;
    t.tomorrow_max = vm.weather.tomorrowMaxC;
    displayWeather(&t);
  }

  if (vm.network.connected && startpage) {
    setBGLight(bright);  // Also lets the ambient-light mode re-sample
    displayDateTime();   // Partial update, no full redraw
  }

  last_ = vm;
  haveLast_ = true;
}

void LvglIli9341Display::loop() {
  touch_loop();

  // Sub pages (favorites, settings, alarm) time out after 10 s.
  if (!startpage && (millis() - start_conf) > 10000) showStartPage();

  // Brightness 0 % means "follow the ambient light sensor".
  if (bright == 0) {
    int16_t tmp = analogRead(PIN_LDR);
    int16_t diff = abs(lastldr - tmp);
    if (startpage && diff > 50) {
      setBGLight(bright);
      lastldr = tmp;
    }
  }
}

void LvglIli9341Display::onLanguageChanged() { tft_i18n_refresh_labels(); }
