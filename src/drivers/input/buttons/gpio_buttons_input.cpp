#include "drivers/input/buttons/gpio_buttons_input.h"

#include <Arduino.h>

namespace {

constexpr uint16_t kRepeatDelayMs = 400;

}  // namespace

GpioButtonsInput::GpioButtonsInput(const ButtonConfig* buttons, uint8_t count)
    : buttons_(buttons), count_(count > kMaxButtons ? kMaxButtons : count) {
  for (uint8_t i = 0; i < count_; i++) {
    ButtonTimings timings;
    if (buttons_[i].repeat) timings.repeatDelayMs = kRepeatDelayMs;
    trackers_[i] = ButtonTracker(timings);
  }
}

bool GpioButtonsInput::begin() {
  for (uint8_t i = 0; i < count_; i++) pinMode(buttons_[i].pin, INPUT_PULLUP);
  return true;
}

bool GpioButtonsInput::poll(InputEvent& out) {
  const uint32_t now = millis();
  for (uint8_t i = 0; i < count_; i++) {
    const bool pressed = digitalRead(buttons_[i].pin) == LOW;
    if (trackers_[i].update(pressed, now)) {
      out.type = buttons_[i].type;
      out.value = buttons_[i].value;
      return true;
    }
  }
  return false;
}
