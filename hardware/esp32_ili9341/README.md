# ESP32 + ILI9341 (original hardware)

ESP32 DevKit, 2.8" ILI9341 TFT with XPT2046 touch (e.g. AZ-Touch MOD), MAX98357A I2S amplifier, optional LDR.
PlatformIO environment: `esp32_ili9341`.

This is the reference configuration inherited from the upstream project. No device is available for
testing in this fork, so it is only guaranteed to compile.

## Pins

| Function | GPIO |
|---|---|
| I2S LRCLK / BCLK / DOUT | 25 / 26 / 33 |
| TFT CS / DC / RST | 5 / 4 / 22 |
| TFT backlight (active low) | 15 |
| Touch CS / IRQ | 14 / 27 |
| LDR | 36 |

SPI uses the default VSPI pins of the ESP32 (SCK 18, MISO 19, MOSI 23).

Wiring diagrams (Fritzing) will move here from `DOKU/Cheap` and `DOKU/Min`.
