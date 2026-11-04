// Next-alarm selection as a pure function so it can be unit-tested on the host.
#pragma once

#include <stddef.h>
#include <stdint.h>

constexpr uint16_t kMinutesPerDay = 1440;
constexpr uint8_t kNoAlarmDay = 8;  // Weekday value meaning "no alarm scheduled"

struct AlarmSlot {
  uint16_t minutes;  // Minutes after midnight; 0 means the slot is unset
  uint8_t daysMask;  // Bit n set = active on weekday n (0 = Sunday)
};

struct NextAlarm {
  uint8_t day;       // 0-6, or kNoAlarmDay if none
  uint16_t minutes;  // Minutes after midnight; kMinutesPerDay + 1 if none
};

// Picks the earliest alarm after (weekday, nowMinutes), looking up to a full week ahead.
// An alarm exactly at nowMinutes counts as already passed; the same weekday next week is a valid result.
NextAlarm computeNextAlarm(const AlarmSlot* slots, size_t count, uint8_t weekday, uint16_t nowMinutes);
