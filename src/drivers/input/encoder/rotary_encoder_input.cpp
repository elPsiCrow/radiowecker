#include "drivers/input/encoder/rotary_encoder_input.h"

#include <Arduino.h>

namespace {

// Shared with the pin interrupts, hence static and volatile.
volatile int16_t pendingDetents = 0;
QuadratureDecoder decoder;
uint8_t isrPinA = 0;
uint8_t isrPinB = 0;

void IRAM_ATTR onEncoderEdge() {
  const int8_t step = decoder.update(digitalRead(isrPinA), digitalRead(isrPinB));
  if (step != 0) pendingDetents = pendingDetents + step;
}

}  // namespace

RotaryEncoderInput::RotaryEncoderInput(uint8_t pinA, uint8_t pinB, int8_t pinSwitch, int16_t volumeStep,
                                       int8_t transitionsPerDetent)
    : pinA_(pinA),
      pinB_(pinB),
      pinSwitch_(pinSwitch),
      volumeStep_(volumeStep),
      transitionsPerDetent_(transitionsPerDetent) {}

bool RotaryEncoderInput::begin() {
  pinMode(pinA_, INPUT_PULLUP);
  pinMode(pinB_, INPUT_PULLUP);
  if (pinSwitch_ >= 0) pinMode((uint8_t)pinSwitch_, INPUT_PULLUP);

  decoder = QuadratureDecoder(transitionsPerDetent_);
  decoder.reset(digitalRead(pinA_), digitalRead(pinB_));
  isrPinA = pinA_;
  isrPinB = pinB_;
  attachInterrupt(digitalPinToInterrupt(pinA_), onEncoderEdge, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB_), onEncoderEdge, CHANGE);
  return true;
}

bool RotaryEncoderInput::poll(InputEvent& out) {
  noInterrupts();
  const int16_t detents = pendingDetents;
  pendingDetents = 0;
  interrupts();

  if (detents != 0) {
    out.type = InputEvent::Type::VolumeDelta;
    out.value = (int16_t)(detents * volumeStep_);
    return true;
  }

  if (pinSwitch_ >= 0 && switchTracker_.update(digitalRead((uint8_t)pinSwitch_) == LOW, millis())) {
    out.type = InputEvent::Type::PlayPause;
    out.value = 0;
    return true;
  }
  return false;
}
