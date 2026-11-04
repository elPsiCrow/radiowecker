// Decodes the two-bit gray code of a rotary encoder into detent steps. Header-only so it can run in an ISR.
#pragma once

#include <stdint.h>

#ifndef IRAM_ATTR
#define IRAM_ATTR  // Host builds have no IRAM
#endif

class QuadratureDecoder {
 public:
  // Encoders differ in how many gray-code transitions one mechanical click produces; 4 is the common case.
  explicit QuadratureDecoder(int8_t transitionsPerDetent = 4) : perDetent_(transitionsPerDetent) {}

  // Sets the starting levels so the first real edge is not mistaken for movement.
  void reset(bool a, bool b) {
    prevA_ = a;
    prevB_ = b;
    accumulated_ = 0;
  }

  // Feed the current levels of A and B. Returns +1 or -1 when a full detent was completed, else 0.
  // Transitions where both lines changed at once (contact bounce) are ignored.
  int8_t IRAM_ATTR update(bool a, bool b) {
    const int step = (prevA_ ^ b) - (prevB_ ^ a);  // +1 clockwise, -1 counter-clockwise, 0 invalid or unchanged
    prevA_ = a;
    prevB_ = b;
    accumulated_ += step;
    if (accumulated_ >= perDetent_) {
      accumulated_ = 0;
      return 1;
    }
    if (accumulated_ <= -perDetent_) {
      accumulated_ = 0;
      return -1;
    }
    return 0;
  }

 private:
  int8_t perDetent_;
  int8_t accumulated_ = 0;
  bool prevA_ = false;
  bool prevB_ = false;
};
