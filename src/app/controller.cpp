#include "app/controller.h"

#include "app/alarm.h"
#include "app/app_config.h"
#include "app/app_state.h"
#include "app/log.h"
#include "audio/audio.h"

namespace {

constexpr uint8_t kMaxListeners = 4;
StateChangeListener listeners[kMaxListeners];
uint8_t listenerCount = 0;

void notify(StateChange change) {
  for (uint8_t i = 0; i < listenerCount; i++) listeners[i](change);
}

bool alarmSoundActive() { return (radio || alarmBeepActive) && alarmActionsVisible; }

}  // namespace

void addStateChangeListener(StateChangeListener listener) {
  if (listenerCount < kMaxListeners) listeners[listenerCount++] = listener;
}

void playbackStart(bool useAlarmGain) {
  if (connected) {
    if (startUrl(stationlist[actStation].url, useAlarmGain)) {
      radio = true;
    } else {
      radio = false;
      RADIO_SERIAL(Serial.println(F("playbackStart: stream start failed")));
    }
  }
  notify(StateChange::Playback);
}

void playbackStop(bool keepAlarmSnoozePending) {
  stopPlaying();
  snoozeTimeEnd = 0;
  if (!keepAlarmSnoozePending) {
    alarmSnoozeUntil = 0;
    alarmActionsVisible = false;
  }
  radio = false;
  setGain(curGain);
  notify(StateChange::Playback);
}

void playbackToggle() {
  if (radio || alarmBeepActive) {
    playbackStop();
  } else {
    playbackStart();
  }
}

void alarmStop() { playbackStop(false); }

void alarmSnooze() {
  if (alarmSnoozeMin == 0) return;
  alarmSnoozeUntil = millis() + (uint32_t)alarmSnoozeMin * 60000UL;
  alarmActionsVisible = false;
  playbackStop(true);
}

void alarmToggleArmed() {
  if (alarmday > 7) {
    findNextAlarm();
    pref.putBool("alarmon", true);
  } else {
    alarmday = 8;
    pref.putBool("alarmon", false);
  }
  notify(StateChange::AlarmArmed);
}

void sleepTimerStart() {
  const uint32_t duration = (uint32_t)snoozeTime * 60000UL;
  if (radio || alarmBeepActive) {
    snoozeTimeEnd = millis() + duration;
  } else {
    playbackStart(false);
    snoozeTimeEnd = millis() + duration;
  }
  notify(StateChange::SleepTimer);
}

void stationApply() {
  actStation = curStation;
  pref.putUShort("station", curStation);
  if (startUrl(stationlist[actStation].url, alarmActionsVisible && radio)) {
    radio = true;
  } else {
    radio = false;
    RADIO_SERIAL(Serial.println(F("stationApply: stream start failed (station selection is still saved)")));
  }
  notify(StateChange::Station);
}

bool stationStep(int direction) {
  const int step = direction < 0 ? STATIONS - 1 : 1;  // Modular arithmetic avoids negative indices
  for (int attempts = 0; attempts < STATIONS; attempts++) {
    curStation = (uint8_t)((curStation + step) % STATIONS);
    if (stationlist[curStation].enabled) {
      stationApply();
      return true;
    }
  }
  RADIO_SERIAL(Serial.println(F("No enabled station found.")));
  return false;
}

bool stationSelect(uint8_t index) {
  if (index >= STATIONS || !stationlist[index].enabled) return false;
  curStation = index;
  stationApply();
  return true;
}

void setVolume(uint8_t percent) {
  curGain = (uint8_t)constrain((int)percent, 0, 100);
  pref.putUShort("gain", curGain);
  setGain(alarmSoundActive() ? alarmGain : curGain);
  notify(StateChange::Volume);
}

void adjustVolume(uint8_t percent) {
  if (alarmSoundActive()) {
    setAlarmVolume(percent);
    return;
  }
  curGain = (uint8_t)constrain((int)percent, 0, 100);
  pref.putUShort("gain", curGain);
  setGain(curGain);
  notify(StateChange::Volume);
}

void adjustVolumeBy(int deltaPercent) {
  const int current = alarmSoundActive() ? alarmGain : curGain;
  adjustVolume((uint8_t)constrain(current + deltaPercent, 0, 100));
}

void setAlarmVolume(uint8_t percent) {
  alarmGain = (uint8_t)constrain((int)percent, 0, 100);
  pref.putUShort("alm_gain", alarmGain);
  if (alarmSoundActive()) setGain(alarmGain);
  notify(StateChange::AlarmVolume);
}

void setBrightnessPercent(uint8_t percent) {
  bright = (uint8_t)constrain((int)percent, 0, 100);
  pref.putUShort("bright", bright);
  notify(StateChange::Brightness);
}

void setSleepTimerMinutes(uint8_t minutes) {
  snoozeTime = (uint8_t)constrain((int)minutes, 0, 60);
  pref.putUShort("snooze", snoozeTime);
  notify(StateChange::SleepTimerMinutes);
}

void setAlarmSnoozeMinutes(uint8_t minutes) {
  alarmSnoozeMin = (uint8_t)constrain((int)minutes, 0, 10);
  pref.putUShort("alm_snooze", alarmSnoozeMin);
  notify(StateChange::AlarmSnoozeMinutes);
}

void setAlarmWakeMode(uint8_t mode) {
  alarmWakeMode = (uint8_t)(mode & 1u);
  pref.putUChar("alm_wake", alarmWakeMode);
  notify(StateChange::AlarmWakeMode);
}
