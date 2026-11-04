#include "app/alarm_schedule.h"

namespace {

// Keeps `best` if it is earlier than the slot's time on `day`. nowMinutes < 0 accepts any time that day.
void consider(const AlarmSlot& slot, int nowMinutes, uint8_t day, uint8_t today, NextAlarm& best) {
  if (slot.minutes == 0) return;
  if (!(slot.daysMask & (1u << day))) return;
  if (day == today && slot.minutes <= nowMinutes) return;
  if (slot.minutes < best.minutes) {
    best.minutes = slot.minutes;
    best.day = day;
  }
}

}  // namespace

NextAlarm computeNextAlarm(const AlarmSlot* slots, size_t count, uint8_t weekday, uint16_t nowMinutes) {
  NextAlarm best{kNoAlarmDay, static_cast<uint16_t>(kMinutesPerDay + 1)};

  for (size_t i = 0; i < count; i++) consider(slots[i], nowMinutes, weekday, weekday, best);
  if (best.day != kNoAlarmDay) return best;

  // Nothing left today: first day within the next 7 that has an alarm; the day itself is searched fresh.
  for (uint8_t offset = 1; offset <= 7; offset++) {
    uint8_t day = static_cast<uint8_t>((weekday + offset) % 7);
    best = NextAlarm{kNoAlarmDay, static_cast<uint16_t>(kMinutesPerDay + 1)};
    for (size_t i = 0; i < count; i++) consider(slots[i], -1, day, weekday, best);
    if (best.day != kNoAlarmDay) return best;
  }
  return best;
}
