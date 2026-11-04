#include "drivers/display/serial/serial_display.h"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#include "app/app_config.h"
#include "board_config.h"
#include "ui/i18n.h"

namespace {

constexpr unsigned long kBaudRate = 115200;
constexpr int kProgressStep = 10;  // Print every 10 % so a firmware update does not flood the console

#if ARDUINO_USB_CDC_ON_BOOT
// Native USB: the host opens the port after the board has booted, so give it a moment to avoid losing the banner.
constexpr uint32_t kUsbConnectWaitMs = 2000;
#endif

}  // namespace

DisplayCapabilities SerialDisplay::capabilities() const {
  DisplayCapabilities caps;
  caps.width = 80;  // Characters per line; there is no fixed height
  return caps;
}

bool SerialDisplay::begin() {
  Serial.begin(kBaudRate);
#if ARDUINO_USB_CDC_ON_BOOT
  for (uint32_t start = millis(); !Serial && millis() - start < kUsbConnectWaitMs;) delay(10);
#endif
  Serial.printf("\nRadiowecker %s on %s (type 'i' for status, any other key for help)\n", RADIOVERSION, BOARD_NAME);
  return true;
}

void SerialDisplay::render(const ViewModel& vm) {
  if (statusShown_) {
    statusShown_ = false;
    lastLine_[0] = '\0';  // Returning from a status screen: print the home line again
  }

  char time[6] = "--:--";
  if (vm.clock.valid) snprintf(time, sizeof(time), "%02u:%02u", vm.clock.hour % 24, vm.clock.minute % 60);

  char alarm[24] = "alarm off";
  if (vm.alarm.armed) {
    snprintf(alarm, sizeof(alarm), "alarm %s %02u:%02u", i18n_day_short(vm.alarm.weekday), vm.alarm.minutes / 60,
             vm.alarm.minutes % 60);
  }

  char playing[170];
  if (vm.playback == PlaybackState::Playing) {
    snprintf(playing, sizeof(playing), "PLAY %s%s%s", vm.stationName, vm.streamTitle[0] ? " - " : "", vm.streamTitle);
  } else {
    snprintf(playing, sizeof(playing), "STOP (%s)", vm.stationName);
  }

  char line[sizeof(lastLine_)];
  snprintf(line, sizeof(line), "[%s] %s | vol %u | %s%s | %s", time, playing, vm.volume, alarm,
           vm.sleepTimerActive ? " | sleep timer" : "", vm.network.connected ? vm.network.ip : "wifi down");

  // Only print when something visible changed; the clock changes once a minute.
  if (strcmp(line, lastLine_) != 0) {
    Serial.println(line);
    strlcpy(lastLine_, line, sizeof(lastLine_));
  }
}

void SerialDisplay::showStatus(const StatusScreen& status) {
  statusShown_ = true;

  if (status.progressPercent >= 0) {
    snprintf(lastStatus_, sizeof(lastStatus_), "== %s: %d %%", status.title, status.progressPercent);
    const bool print = lastProgress_ < 0 || status.progressPercent / kProgressStep != lastProgress_ / kProgressStep ||
                       status.progressPercent == 100;
    lastProgress_ = status.progressPercent;
    if (print) Serial.println(lastStatus_);
    return;
  }
  lastProgress_ = -1;

  int len = snprintf(lastStatus_, sizeof(lastStatus_), "== %s%s ==", status.tone == StatusTone::Error ? "ERROR: " : "",
                     status.title);
  for (uint8_t i = 0; i < status.lineCount && i < StatusScreen::kMaxLines && len < (int)sizeof(lastStatus_); i++) {
    len += snprintf(lastStatus_ + len, sizeof(lastStatus_) - len, "\n   %s", status.lines[i]);
  }
  Serial.println(lastStatus_);
}

void SerialDisplay::redraw() {
  if (statusShown_) {
    Serial.println(lastStatus_);
  } else {
    lastLine_[0] = '\0';  // The next render() prints the home line again
  }
}
