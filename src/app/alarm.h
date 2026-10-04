// Next-alarm calculation.
#pragma once

void updateCurrentTime();  // Refresh weekday/minutes from the system clock
void findNextAlarm();      // Set alarmday/alarmtime to the next enabled alarm, or alarmday = 8 if none
