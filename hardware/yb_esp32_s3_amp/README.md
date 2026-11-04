# YB-ESP32-S3-AMP Rev.3

Deutsch: [README.de.md](README.de.md).

Development board by yellobyte: ESP32-S3-WROOM-1 (N8R2: 8 MB flash, 2 MB QSPI PSRAM), two MAX98357A
amplifiers, microSD slot and native USB serial/JTAG.
Board documentation: <https://github.com/yellobyte/YB-ESP32-S3-AMP>

PlatformIO environment: `yb_esp32_s3_amp` (`board = yb_esp32s3_amp_v3`).

## Fixed pins

| Function | GPIO | Notes |
| --- | --- | --- |
| I2S BCLK | 5 | Not routed to a header pin |
| I2S LRCLK | 6 | Not routed to a header pin |
| I2S DOUT | 7 | Not routed to a header pin |
| SD CS / MOSI / SCK / MISO | 10 / 11 / 12 / 13 | FSPI; 11-13 not routed to a header pin |
| Status LED | 47 | Doubles as amplifier mute if solder bridge `DAC_MUTE` is closed (default open) |
| Boot button | 0 | |

## Serial output

Rev.3 uses the ESP32-S3 native USB CDC port for `Serial`. The monitor may disconnect and reconnect when
the board resets; this is expected. The firmware waits up to two seconds for the monitor before printing
its startup banner. Flashing is automatic; if it fails, hold `B`, tap `R`, then release `B`.

## First run

1. `pio run -e yb_esp32_s3_amp -t upload`, then `pio device monitor -e yb_esp32_s3_amp` (115200 baud).
2. Without stored Wi-Fi credentials the console prints `== Not connected ==` and the soft-AP name
   `radioweckerconf`. Join that network and open `http://192.168.4.1` to enter your Wi-Fi settings.
3. After the restart the console prints a status line whenever something changes. Type `p` + Enter to
   play, `n` for the next station, `v40` to set the volume; any other input lists all commands
   (see [serial input](../../src/drivers/input/serial/README.md)).

## Other module variants

Other module variants may need different flash and PSRAM settings. Select the matching official board
environment and verify its build settings before flashing.
