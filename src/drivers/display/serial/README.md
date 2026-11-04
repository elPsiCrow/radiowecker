# Serial console display

Text-only display that prints one status line to the serial monitor whenever something changes
(clock minute, playback, station, title, volume, alarm, Wi-Fi). Status screens such as Wi-Fi setup and
firmware-update progress are printed as `== title ==` blocks.

Needs no hardware beyond the USB serial connection, so it works on every board. The baud rate is 115200.
Pair it with the [serial input](../../input/serial/README.md) to control the radio from the same terminal.
