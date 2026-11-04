#include <unity.h>

#include "app/alarm_schedule.h"

// Weekdays: 0 = Sunday ... 6 = Saturday. Day masks: bit n = weekday n.
constexpr uint8_t kMonToFri = 0b00111110;
constexpr uint8_t kWeekend = 0b01000001;
constexpr uint8_t kEveryDay = 0b01111111;

void setUp() {}
void tearDown() {}

static void expectAlarm(NextAlarm actual, uint8_t day, uint16_t minutes) {
  TEST_ASSERT_EQUAL_UINT8(day, actual.day);
  TEST_ASSERT_EQUAL_UINT16(minutes, actual.minutes);
}

void test_no_slots_returns_none() {
  expectAlarm(computeNextAlarm(nullptr, 0, 3, 600), kNoAlarmDay, kMinutesPerDay + 1);
}

void test_unset_slot_is_ignored() {
  const AlarmSlot slots[] = {{0, kEveryDay}};
  expectAlarm(computeNextAlarm(slots, 1, 3, 600), kNoAlarmDay, kMinutesPerDay + 1);
}

void test_slot_without_days_is_ignored() {
  const AlarmSlot slots[] = {{450, 0}};
  expectAlarm(computeNextAlarm(slots, 1, 3, 600), kNoAlarmDay, kMinutesPerDay + 1);
}

void test_later_today() {
  const AlarmSlot slots[] = {{390, kMonToFri}};
  expectAlarm(computeNextAlarm(slots, 1, 2, 300), 2, 390);  // Tuesday 05:00 -> Tuesday 06:30
}

void test_earlier_of_two_slots_today() {
  const AlarmSlot slots[] = {{480, kEveryDay}, {390, kEveryDay}};
  expectAlarm(computeNextAlarm(slots, 2, 2, 300), 2, 390);
}

void test_alarm_at_current_minute_is_skipped() {
  const AlarmSlot slots[] = {{390, kMonToFri}};
  expectAlarm(computeNextAlarm(slots, 1, 2, 390), 3, 390);  // Tuesday 06:30 -> Wednesday 06:30
}

void test_passed_today_rolls_to_next_enabled_day() {
  const AlarmSlot slots[] = {{390, kMonToFri}};
  expectAlarm(computeNextAlarm(slots, 1, 5, 700), 1, 390);  // Friday 11:40 -> Monday (skips weekend)
}

void test_weekend_slot_from_weekday() {
  const AlarmSlot slots[] = {{480, kWeekend}};
  expectAlarm(computeNextAlarm(slots, 1, 3, 100), 6, 480);  // Wednesday -> Saturday
}

void test_week_wraps_past_saturday() {
  const AlarmSlot slots[] = {{390, kMonToFri}};
  expectAlarm(computeNextAlarm(slots, 1, 6, 900), 1, 390);  // Saturday evening -> Monday
}

void test_same_weekday_next_week() {
  const AlarmSlot slots[] = {{390, 0b00000100}};  // Tuesdays only
  expectAlarm(computeNextAlarm(slots, 1, 2, 800), 2, 390);
}

void test_next_day_prefers_earlier_slot() {
  const AlarmSlot slots[] = {{480, kEveryDay}, {390, kEveryDay}};
  expectAlarm(computeNextAlarm(slots, 2, 2, 1000), 3, 390);
}

void test_slot_later_today_beats_tomorrow() {
  const AlarmSlot slots[] = {{1380, kEveryDay}, {60, kEveryDay}};
  expectAlarm(computeNextAlarm(slots, 2, 2, 1000), 2, 1380);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_no_slots_returns_none);
  RUN_TEST(test_unset_slot_is_ignored);
  RUN_TEST(test_slot_without_days_is_ignored);
  RUN_TEST(test_later_today);
  RUN_TEST(test_earlier_of_two_slots_today);
  RUN_TEST(test_alarm_at_current_minute_is_skipped);
  RUN_TEST(test_passed_today_rolls_to_next_enabled_day);
  RUN_TEST(test_weekend_slot_from_weekday);
  RUN_TEST(test_week_wraps_past_saturday);
  RUN_TEST(test_same_weekday_next_week);
  RUN_TEST(test_next_day_prefers_earlier_slot);
  RUN_TEST(test_slot_later_today_beats_tomorrow);
  return UNITY_END();
}
