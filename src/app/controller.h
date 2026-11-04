// Application actions (play, stop, snooze, volume, ...) that change state and persist settings.
// Inputs (touch, buttons, web) call these; displays learn about the result through state-change listeners.
#pragma once

#include <stdint.h>

enum class StateChange : uint8_t {
  Playback,             // Radio or alarm sound started or stopped
  Station,              // Active station changed
  AlarmArmed,           // Alarm switched on or off
  SleepTimer,           // Sleep timer started
  Volume,               // Normal volume changed
  AlarmVolume,          // Alarm volume changed
  Brightness,           // Display brightness setting changed
  SleepTimerMinutes,    // Sleep timer duration setting changed
  AlarmSnoozeMinutes,   // Alarm snooze duration setting changed
  AlarmWakeMode,        // Alarm sound (radio or beep) changed
};

typedef void (*StateChangeListener)(StateChange change);

// Up to four listeners (e.g. TFT, OLED, serial console). Called synchronously; callbacks must stay short.
void addStateChangeListener(StateChangeListener listener);

// Playback
void playbackStart(bool useAlarmGain = false);
void playbackStop(bool keepAlarmSnoozePending = false);
void playbackToggle();

// Alarm
void alarmStop();
void alarmSnooze();         // No effect if the snooze duration is 0
void alarmToggleArmed();    // Arms (and finds the next alarm) or disarms
void sleepTimerStart();     // Starts the radio if needed, then switches it off after the sleep-timer minutes

// Stations. curStation is the selection, actStation the one that is streaming.
void stationApply();                 // Make curStation the active station and persist it
bool stationStep(int direction);     // -1 / +1: next enabled station; false if none is enabled
bool stationSelect(uint8_t index);   // False if index is out of range or the station is disabled

// Settings. Each one validates, stores to NVS and notifies listeners.
void setVolume(uint8_t percent);         // Web semantics: always edits the normal volume
void adjustVolume(uint8_t percent);      // Slider semantics: edits the alarm volume while the alarm is ringing
void adjustVolumeBy(int deltaPercent);   // Same target as adjustVolume, relative to its current value
void setAlarmVolume(uint8_t percent);
void setBrightnessPercent(uint8_t percent);  // 0 = automatic
void setSleepTimerMinutes(uint8_t minutes);
void setAlarmSnoozeMinutes(uint8_t minutes);
void setAlarmWakeMode(uint8_t mode);     // 0 = radio, 1 = beep
