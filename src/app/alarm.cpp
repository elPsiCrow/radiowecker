#include "app/alarm.h"

#include "app/alarm_schedule.h"
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

  const AlarmSlot slots[] = {{alarm1, alarmday1}, {alarm2, alarmday2}};
  const NextAlarm next = computeNextAlarm(slots, 2, weekday, minutes);
  alarmday = next.day;
  alarmtime = next.minutes;

  if (alarmday != kNoAlarmDay) {
    RADIO_SERIAL(Serial.printf("Next alarm: %02d:%02d on %s\n", alarmtime / 60, alarmtime % 60,
                               i18n_day_short((int)alarmday)));
  } else {
    RADIO_SERIAL(Serial.println("No alarm found"));
  }
}
