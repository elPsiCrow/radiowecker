// Momentary push buttons on GPIO pins (wired to GND, internal pull-up) mapped to input events.
#pragma once

#include "drivers/input/buttons/button_tracker.h"
#include "ui/input.h"

struct ButtonConfig {
  uint8_t pin;
  InputEvent::Type type;
  int16_t value;  // Event payload, e.g. the volume step for VolumeDelta
  bool repeat;    // Auto-repeat while held (useful for volume buttons)
};

class GpioButtonsInput : public IInput {
 public:
  static constexpr uint8_t kMaxButtons = 8;

  // `buttons` must stay valid for the lifetime of this object (use a static array).
  GpioButtonsInput(const ButtonConfig* buttons, uint8_t count);

  const char* name() const override { return "GPIO buttons"; }
  bool begin() override;
  bool poll(InputEvent& out) override;

 private:
  const ButtonConfig* buttons_;
  uint8_t count_;
  ButtonTracker trackers_[kMaxButtons];
};
