# YB-ESP32-S3-AMP

Development board by yellobyte: ESP32-S3-WROOM-1 (N8R2: 8 MB flash, 2 MB QSPI PSRAM), two MAX98357A
amplifiers, microSD slot, USB-C with CH343 UART bridge and USB-JTAG.
Board documentation: <https://github.com/yellobyte/YB-ESP32-S3-AMP>

PlatformIO environment: `yb_esp32_s3_amp` (board definition in [`boards/`](../../boards/yb_esp32_s3_amp.json)).

## Fixed pins

| Function | GPIO | Notes |
|---|---|---|
| I2S BCLK | 5 | Not routed to a header pin |
| I2S LRCLK | 6 | Not routed to a header pin |
| I2S DOUT | 7 | Not routed to a header pin |
| SD CS / MOSI / SCK / MISO | 10 / 11 / 12 / 13 | FSPI; 11-13 not routed to a header pin |
| Status LED | 47 | Doubles as amplifier mute if solder bridge `DAC_MUTE` is closed (default open) |
| Boot button | 0 | |

## Serial output

The board shows two COM ports. Serial monitor output appears on the "USB-Enhanced-Serial CH343" port;
the Windows driver is available from WCH (search for CH343). Flashing is automatic; if it fails, hold `B`,
tap `R`, release `B`.

## First run

1. `pio run -e yb_esp32_s3_amp -t upload`, then `pio device monitor` on the CH343 port (115200 baud).
2. Without stored Wi-Fi credentials the console prints `== Not connected ==` and the soft-AP name
   `radioweckerconf`. Join that network and open `http://192.168.4.1` to enter your Wi-Fi settings.
3. After the restart the console prints a status line whenever something changes. Type `p` + Enter to
   play, `n` for the next station, `v40` to set the volume; any other input lists all commands
   (see [serial input](../../src/drivers/input/serial/README.md)).

## Other module variants

For -N16R8 / -N8R8 modules the PSRAM mode and flash size in `boards/yb_esp32_s3_amp.json` have to change
(see the board README).
