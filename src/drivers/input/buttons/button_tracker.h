// Debounce and optional auto-repeat for one button, independent of the hardware it is read from.
#pragma once

#include <stdint.h>

struct ButtonTimings {
  uint16_t debounceMs = 30;
  uint16_t repeatDelayMs = 0;      // Hold time before auto-repeat starts; 0 disables repeat
  uint16_t repeatIntervalMs = 150;
};

class ButtonTracker {
 public:
  explicit ButtonTracker(ButtonTimings timings = ButtonTimings()) : timings_(timings) {}

  // Call regularly with the raw state. Returns true once per press, and again on each repeat while held.
  bool update(bool rawPressed, uint32_t nowMs);

 private:
  ButtonTimings timings_;
  bool raw_ = false;
  bool stable_ = false;
  uint32_t rawChangedMs_ = 0;
  uint32_t nextRepeatMs_ = 0;
};
