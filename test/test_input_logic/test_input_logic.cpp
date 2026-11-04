#include <unity.h>

#include "drivers/input/buttons/button_tracker.h"
#include "drivers/input/encoder/quadrature_decoder.h"

void setUp() {}
void tearDown() {}

// Feeds a sequence of (A,B) levels and returns the sum of the returned detent steps.
static int feed(QuadratureDecoder& d, const bool (*levels)[2], int count) {
  int total = 0;
  for (int i = 0; i < count; i++) total += d.update(levels[i][0], levels[i][1]);
  return total;
}

// One clockwise cycle starting from the idle state 00: 01, 11, 10, 00.
static const bool kClockwise[4][2] = {{false, true}, {true, true}, {true, false}, {false, false}};
// One counter-clockwise cycle: 10, 11, 01, 00.
static const bool kCounterClockwise[4][2] = {{true, false}, {true, true}, {false, true}, {false, false}};

void test_clockwise_cycle_gives_one_detent() {
  QuadratureDecoder d;
  TEST_ASSERT_EQUAL_INT(1, feed(d, kClockwise, 4));
}

void test_counter_clockwise_cycle_gives_minus_one() {
  QuadratureDecoder d;
  TEST_ASSERT_EQUAL_INT(-1, feed(d, kCounterClockwise, 4));
}

void test_three_cycles_give_three_detents() {
  QuadratureDecoder d;
  int total = 0;
  for (int i = 0; i < 3; i++) total += feed(d, kClockwise, 4);
  TEST_ASSERT_EQUAL_INT(3, total);
}

void test_partial_turn_gives_no_detent() {
  QuadratureDecoder d;
  TEST_ASSERT_EQUAL_INT(0, feed(d, kClockwise, 2));
}

void test_back_and_forth_cancels_out() {
  QuadratureDecoder d;
  // Two steps forward then two back should not produce a detent.
  TEST_ASSERT_EQUAL_INT(0, feed(d, kClockwise, 2));
  static const bool back[2][2] = {{false, true}, {false, false}};
  TEST_ASSERT_EQUAL_INT(0, feed(d, back, 2));
}

void test_simultaneous_change_is_ignored() {
  QuadratureDecoder d;
  // Both lines flipping at once cannot happen on a real encoder; it must not count as movement.
  TEST_ASSERT_EQUAL_INT(0, d.update(true, true));
}

void test_two_transitions_per_detent() {
  QuadratureDecoder d(2);
  TEST_ASSERT_EQUAL_INT(2, feed(d, kClockwise, 4));
}

void test_button_fires_after_debounce() {
  ButtonTracker b;
  TEST_ASSERT_FALSE(b.update(true, 100));
  TEST_ASSERT_FALSE(b.update(true, 120));
  TEST_ASSERT_TRUE(b.update(true, 131));
  TEST_ASSERT_FALSE(b.update(true, 400));  // No repeat configured
}

void test_bounce_is_filtered() {
  ButtonTracker b;
  TEST_ASSERT_FALSE(b.update(true, 100));
  TEST_ASSERT_FALSE(b.update(false, 110));  // Released before the debounce time elapsed
  TEST_ASSERT_FALSE(b.update(false, 200));
  TEST_ASSERT_FALSE(b.update(true, 210));
}

void test_release_does_not_fire() {
  ButtonTracker b;
  b.update(true, 0);
  TEST_ASSERT_TRUE(b.update(true, 40));
  b.update(false, 100);
  TEST_ASSERT_FALSE(b.update(false, 140));
}

void test_repeat_while_held() {
  ButtonTimings t;
  t.repeatDelayMs = 400;
  t.repeatIntervalMs = 100;
  ButtonTracker b(t);
  b.update(true, 0);
  TEST_ASSERT_TRUE(b.update(true, 40));     // Press
  TEST_ASSERT_FALSE(b.update(true, 300));   // Still in the initial delay
  TEST_ASSERT_TRUE(b.update(true, 440));    // First repeat
  TEST_ASSERT_FALSE(b.update(true, 500));
  TEST_ASSERT_TRUE(b.update(true, 540));    // Next repeat
}

void test_timer_wraparound() {
  ButtonTracker b;
  const uint32_t nearWrap = 0xFFFFFFF0u;
  TEST_ASSERT_FALSE(b.update(true, nearWrap));
  TEST_ASSERT_TRUE(b.update(true, nearWrap + 40u));  // Wraps past zero
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_clockwise_cycle_gives_one_detent);
  RUN_TEST(test_counter_clockwise_cycle_gives_minus_one);
  RUN_TEST(test_three_cycles_give_three_detents);
  RUN_TEST(test_partial_turn_gives_no_detent);
  RUN_TEST(test_back_and_forth_cancels_out);
  RUN_TEST(test_simultaneous_change_is_ignored);
  RUN_TEST(test_two_transitions_per_detent);
  RUN_TEST(test_button_fires_after_debounce);
  RUN_TEST(test_bounce_is_filtered);
  RUN_TEST(test_release_does_not_fire);
  RUN_TEST(test_repeat_while_held);
  RUN_TEST(test_timer_wraparound);
  return UNITY_END();
}
