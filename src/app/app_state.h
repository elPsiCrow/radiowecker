// Shared application state. Phase 2 replaces these loose globals with structured state.
#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <time.h>

#include "app/app_config.h"
#include "app/stations.h"

extern Preferences pref;    // NVS namespace "radio"
extern Preferences sender;  // NVS namespace "senderlist"

extern Station stationlist[STATIONS];

// Persistent configuration
extern char ssid[CFG_SSID_MAX];
extern char pkey[CFG_WPA_PSK_MAX];
extern char ntp[CFG_NTP_HOST_MAX];
extern float LATITUDE;
extern float LONGITUDE;
extern char TIME_ZONE_IANA[CFG_TZ_IANA_MAX];

extern uint8_t curStation;       // Station selected in the UI
extern uint8_t actStation;       // Station that is actually streaming
extern uint8_t curGain;          // Volume 0-100
extern uint8_t snoozeTime;       // Sleep timer in minutes
extern uint8_t alarmSnoozeMin;   // Alarm snooze in minutes, 0 = disabled
extern uint8_t alarmGain;        // Volume while the alarm plays, 0-100
extern uint8_t alarmWakeMode;    // 0 = radio, 1 = beep
extern uint8_t bright;           // Brightness in percent; 0 = automatic via LDR
extern uint16_t alarm1;          // Alarm times in minutes after midnight
extern uint16_t alarm2;
extern uint8_t alarmday1;        // Weekday bit masks (bit 0 = Sunday)
extern uint8_t alarmday2;

// Runtime state
extern bool alarmBeepActive;
extern uint32_t lastchange;
extern uint32_t snoozeTimeEnd;       // millis() when the sleep timer ends, 0 = inactive
extern uint32_t alarmSnoozeUntil;    // millis() when the alarm snooze ends, 0 = inactive
extern bool alarmActionsVisible;     // Stop/snooze buttons shown (alarm started or snooze elapsed)
extern uint16_t alarmtime;           // Next alarm, minutes after midnight
extern uint8_t alarmday;             // Next alarm weekday; 8 = none
extern char title[128];              // Stream title, UTF-8
extern bool newTitle;
extern uint32_t tick;
extern uint32_t discon;              // millis() at the last disconnect
extern uint16_t minutes;             // Current time, minutes after midnight
extern uint8_t weekday;              // 0 = Sunday
extern struct tm ti;
extern int16_t lastldr;
extern uint32_t start_conf;          // millis() of the last switch to a sub page (timeout returns to start page)
extern boolean connected;
extern boolean reconnected;
extern boolean radio;
extern boolean startpage;
extern boolean favoritenpage;
extern boolean settingspage;
extern boolean alarmpage;
extern boolean alarmgainpage;
extern unsigned long lastUpdate;
extern uint16_t lastLedb;
extern bool alarmTriggered;
extern unsigned long lastWeatherUpdate;
