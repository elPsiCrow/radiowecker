#include "app/view_model_builder.h"

#include <WiFi.h>
#include <string.h>

#include "app/app_state.h"

namespace {

// ti is refreshed by the main loop; a year before 2020 means SNTP has not set the clock yet.
constexpr int kFirstValidYear = 120;  // tm_year counts from 1900

}  // namespace

void buildViewModel(ViewModel& vm) {
  vm.clock.valid = ti.tm_year >= kFirstValidYear;
  vm.clock.hour = (uint8_t)ti.tm_hour;
  vm.clock.minute = (uint8_t)ti.tm_min;
  vm.clock.weekday = (uint8_t)ti.tm_wday;
  vm.clock.day = (uint8_t)ti.tm_mday;
  vm.clock.month = (uint8_t)ti.tm_mon;
  vm.clock.year = (uint16_t)(ti.tm_year + 1900);

  vm.weather = weatherState;

  vm.alarm.armed = pref.isKey("alarmon") && pref.getBool("alarmon") && alarmday < 8;
  vm.alarm.weekday = alarmday;
  vm.alarm.minutes = alarmtime;
  vm.alarm.ringing = alarmActionsVisible;
  vm.alarm.snoozed = alarmSnoozeUntil != 0;

  vm.network.connected = connected;
  vm.network.rssiDbm = connected ? (int8_t)WiFi.RSSI() : 0;
  if (connected) {
    strlcpy(vm.network.ip, WiFi.localIP().toString().c_str(), sizeof(vm.network.ip));
  } else {
    vm.network.ip[0] = '\0';
  }

  strlcpy(vm.stationName, stationlist[actStation].name, sizeof(vm.stationName));
  strlcpy(vm.streamTitle, radio ? title : "", sizeof(vm.streamTitle));
  vm.playback = (radio || alarmBeepActive) ? PlaybackState::Playing : PlaybackState::Stopped;
  vm.sleepTimerActive = snoozeTimeEnd != 0;

  vm.volume = (alarmActionsVisible && (radio || alarmBeepActive)) ? alarmGain : curGain;
  vm.brightness = bright;
}
