# GPIO buttons

Momentary push buttons wired between a GPIO pin and GND (the internal pull-up is enabled). Each button is
described by a `ButtonConfig` in the board's `board_setup.cpp`: pin, event, optional value and whether the
event repeats while the button is held. Presses are debounced for 30 ms; repeating buttons start repeating
after 400 ms and then fire every 150 ms.

The debounce and repeat logic lives in `button_tracker.*` and has host-side unit tests
(`pio test -e native`).

Example:

```cpp
static const ButtonConfig buttons[] = {
    {PIN_BTN_PLAY, InputEvent::Type::PlayPause, 0, false},
    {PIN_BTN_VOL_UP, InputEvent::Type::VolumeDelta, +5, true},
    {PIN_BTN_VOL_DOWN, InputEvent::Type::VolumeDelta, -5, true},
};
static GpioButtonsInput input(buttons, 3);
```
