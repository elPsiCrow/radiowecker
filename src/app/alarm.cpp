#include "app/alarm.h"

#include "app/app_config.h"
#include "app/app_state.h"
#include "app/log.h"
#include "ui/i18n.h"

void updateCurrentTime() {
  if (getLocalTime(&ti)) {
    weekday = ti.tm_wday;                   // 0 = Sunday ... 6 = Saturday
    minutes = ti.tm_hour * 60 + ti.tm_min;  // Minutes since midnight
  }
}

void findNextAlarm() {
  RADIO_SERIAL(Serial.println("Search next alarm time"));

  if (!getLocalTime(&ti)) return;  // No valid time yet

  updateCurrentTime();

  alarmday = 8;  // 8 = no alarm
  alarmtime = MINUTES_PER_DAY + 1;

  // Considers one alarm on one weekday and keeps it if it is earlier than the best candidate so far.
  auto checkAlarm = [&](uint8_t daysMask, int alarmMinutes, int currentMinutes, int day) {
    if (alarmMinutes <= 0) return;  // Not set
    if (daysMask & (1 << day)) {
      if (day == weekday) {
        // Today only counts if the time is still ahead.
        if (alarmMinutes > currentMinutes && alarmMinutes < alarmtime) {
          alarmtime = alarmMinutes;
          alarmday = day;
        }
      } else if (alarmMinutes < alarmtime) {
        alarmtime = alarmMinutes;
        alarmday = day;
      }
    }
  };

  checkAlarm(alarmday1, alarm1, minutes, weekday);
  checkAlarm(alarmday2, alarm2, minutes, weekday);

  // Nothing left today: take the first matching day of the next 7.
  if (alarmday == 8) {
    for (int i = 1; i <= 7; i++) {
      int nextDay = (weekday + i) % 7;
      alarmtime = MINUTES_PER_DAY + 1;
      checkAlarm(alarmday1, alarm1, -1, nextDay);
      checkAlarm(alarmday2, alarm2, -1, nextDay);
      if (alarmday != 8) break;
    }
  }

  if (alarmday != 8) {
    RADIO_SERIAL(Serial.printf("Next alarm: %02d:%02d on %s\n", alarmtime / 60, alarmtime % 60,
                               i18n_day_short((int)alarmday)));
  } else {
    RADIO_SERIAL(Serial.println("No alarm found"));
  }
}
