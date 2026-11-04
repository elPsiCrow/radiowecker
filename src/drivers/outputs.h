// Output devices that are not displays: screen backlight and room lights.
#pragma once

#include <stdint.h>

class IBacklight {
 public:
  virtual ~IBacklight() = default;

  virtual bool begin() = 0;

  // percent 1-100 sets a fixed level, 0 selects automatic control (e.g. from an ambient light sensor).
  virtual void setBrightness(uint8_t percent) = 0;

  // Called from the main loop; implementations with automatic control sample their sensor here.
  virtual void loop() {}
};

// A light that can be dimmed, e.g. for a sunrise-style wake-up. The app drives the ramp; the driver only sets levels.
class ILightOutput {
 public:
  virtual ~ILightOutput() = default;

  virtual bool begin() = 0;

  // 0 = off, 1000 = full brightness.
  virtual void setLevel(uint16_t permille) = 0;
};
