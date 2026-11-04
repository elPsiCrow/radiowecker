#include "drivers/input/buttons/button_tracker.h"

bool ButtonTracker::update(bool rawPressed, uint32_t nowMs) {
  if (rawPressed != raw_) {
    raw_ = rawPressed;
    rawChangedMs_ = nowMs;
  }

  // A new state counts once it has been steady for the debounce time.
  if (raw_ != stable_ && (uint32_t)(nowMs - rawChangedMs_) >= timings_.debounceMs) {
    stable_ = raw_;
    if (!stable_) return false;
    nextRepeatMs_ = nowMs + timings_.repeatDelayMs;
    return true;
  }

  if (stable_ && timings_.repeatDelayMs != 0 && (int32_t)(nowMs - nextRepeatMs_) >= 0) {
    nextRepeatMs_ = nowMs + timings_.repeatIntervalMs;
    return true;
  }
  return false;
}
