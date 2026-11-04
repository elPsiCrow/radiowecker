#include "app/board_setup.h"

#include "board_config.h"
#include "drivers/display/serial/serial_display.h"
#include "drivers/input/buttons/gpio_buttons_input.h"
#include "drivers/input/encoder/rotary_encoder_input.h"
#include "drivers/input/serial/serial_input.h"
#include "ui/ui_manager.h"

#if defined(PIN_BTN_PLAY) || defined(PIN_BTN_NEXT) || defined(PIN_BTN_PREV) || defined(PIN_BTN_VOL_UP) || \
    defined(PIN_BTN_VOL_DOWN)
#define HAS_GPIO_BUTTONS 1
#endif

#ifdef HAS_GPIO_BUTTONS
namespace {

constexpr int16_t kVolumeStep = 5;

const ButtonConfig kButtons[] = {
#ifdef PIN_BTN_PLAY
    {PIN_BTN_PLAY, InputEvent::Type::PlayPause, 0, false},
#endif
#ifdef PIN_BTN_NEXT
    {PIN_BTN_NEXT, InputEvent::Type::NextStation, 0, false},
#endif
#ifdef PIN_BTN_PREV
    {PIN_BTN_PREV, InputEvent::Type::PrevStation, 0, false},
#endif
#ifdef PIN_BTN_VOL_UP
    {PIN_BTN_VOL_UP, InputEvent::Type::VolumeDelta, kVolumeStep, true},
#endif
#ifdef PIN_BTN_VOL_DOWN
    {PIN_BTN_VOL_DOWN, InputEvent::Type::VolumeDelta, -kVolumeStep, true},
#endif
};

}  // namespace
#endif

void boardSetup(UiManager& ui) {
  static SerialDisplay display;
  static SerialInput serialInput;
  ui.addDisplay(&display);
  ui.addInput(&serialInput);

#ifdef HAS_GPIO_BUTTONS
  static GpioButtonsInput buttons(kButtons, sizeof(kButtons) / sizeof(kButtons[0]));
  ui.addInput(&buttons);
#endif

#if defined(PIN_ENCODER_A) && defined(PIN_ENCODER_B)
#ifndef PIN_ENCODER_SW
#define PIN_ENCODER_SW (-1)
#endif
  static RotaryEncoderInput encoder(PIN_ENCODER_A, PIN_ENCODER_B, PIN_ENCODER_SW);
  ui.addInput(&encoder);
#endif
}
