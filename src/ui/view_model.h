// Everything a display needs to show, independent of how it is drawn.
// The application fills this in; each display driver decides what to render and how.
#pragma once

#include <stdint.h>

enum class PlaybackState : uint8_t {
  Stopped,
  Playing,  // Radio stream or alarm beep is audible
};

struct ClockView {
  bool valid = false;  // False until the clock has been synchronised
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t weekday = 0;  // 0 = Sunday
  uint8_t day = 1;      // Day of month
  uint8_t month = 0;    // 0 = January
  uint16_t year = 1970;
};

struct WeatherView {
  bool valid = false;
  uint32_t revision = 0;  // Changes with every successful fetch
  float currentC = 0;
  float feelsC = 0;
  float todayMinC = 0;
  float todayMaxC = 0;
  float tomorrowMinC = 0;
  float tomorrowMaxC = 0;
};

struct AlarmView {
  bool armed = false;     // Alarm switch is on and a next alarm exists
  uint8_t weekday = 8;    // Weekday of the next alarm; 8 = none
  uint16_t minutes = 0;   // Minutes after midnight
  bool ringing = false;   // Wake-up sound is active or waiting for stop/snooze
  bool snoozed = false;   // Waiting for the snooze period to end
};

struct NetworkView {
  bool connected = false;
  int8_t rssiDbm = 0;
  char ip[16] = "";  // Dotted quad, empty when not connected
};

struct ViewModel {
  uint32_t revision = 0;  // Increases whenever application state changed; the clock alone does not bump it
  ClockView clock;
  WeatherView weather;
  AlarmView alarm;
  NetworkView network;

  char stationName[32] = "";
  char streamTitle[128] = "";  // UTF-8; empty when the station sends no title
  PlaybackState playback = PlaybackState::Stopped;
  bool sleepTimerActive = false;

  uint8_t volume = 0;      // 0-100
  uint8_t brightness = 0;  // 0-100; 0 = automatic
};

// Transient full-screen message, used during boot, Wi-Fi setup and firmware updates.
enum class StatusTone : uint8_t { Info, Ok, Error };

struct StatusScreen {
  static constexpr uint8_t kMaxLines = 4;
  StatusTone tone = StatusTone::Info;
  const char* title = "";
  const char* lines[kMaxLines] = {};
  uint8_t lineCount = 0;
  int8_t progressPercent = -1;  // -1 = no progress bar
};
