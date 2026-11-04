# Rotary encoder

Quadrature rotary encoder on two GPIO pins (internal pull-ups, common contact to GND), with an optional push
switch. Turning changes the volume by `volumeStep` percent per click; pressing the switch toggles play/pause.
Edges are handled in pin interrupts, so fast turning is not lost while the main loop is busy streaming.

Encoders differ in how many contact transitions make up one click. The default is 4; if one click changes the
volume twice, or two clicks are needed for one step, pass 2 or 4 as `transitionsPerDetent`.

The gray-code decoding is in `quadrature_decoder.h` and has host-side unit tests (`pio test -e native`).
Only one encoder can be used per device.
