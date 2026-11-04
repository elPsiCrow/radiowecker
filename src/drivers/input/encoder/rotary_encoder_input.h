// Rotary encoder with optional push switch. Rotation changes the volume; the switch toggles play/pause.
#pragma once

#include "drivers/input/buttons/button_tracker.h"
#include "drivers/input/encoder/quadrature_decoder.h"
#include "ui/input.h"

class RotaryEncoderInput : public IInput {
 public:
  // pinSwitch < 0 means no push switch. volumeStep is the percent change per detent.
  // Only one instance can exist: the pin interrupts share static state.
  RotaryEncoderInput(uint8_t pinA, uint8_t pinB, int8_t pinSwitch = -1, int16_t volumeStep = 2,
                     int8_t transitionsPerDetent = 4);

  const char* name() const override { return "Rotary encoder"; }
  bool begin() override;
  bool poll(InputEvent& out) override;

 private:
  uint8_t pinA_;
  uint8_t pinB_;
  int8_t pinSwitch_;
  int16_t volumeStep_;
  int8_t transitionsPerDetent_;
  ButtonTracker switchTracker_;
};
