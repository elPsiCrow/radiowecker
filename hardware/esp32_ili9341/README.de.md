# ESP32 + ILI9341 (Referenzhardware)

ESP32 DevKit, 2,8-Zoll-ILI9341-TFT mit XPT2046-Touch (z. B. AZ-Touch MOD), MAX98357A-I2S-Verstärker und optionaler LDR. PlatformIO-Umgebung: `esp32_ili9341`. Englische Version: [README.md](README.md).

Dies ist die Referenzkonfiguration aus dem ursprünglichen Projekt. In diesem Fork steht kein entsprechendes Gerät zum Testen bereit; garantiert ist daher nur, dass die Firmware kompiliert.

## Pins

| Funktion | GPIO |
|---|---:|
| I2S LRCLK / BCLK / DOUT | 25 / 26 / 33 |
| TFT CS / DC / RST | 5 / 4 / 22 |
| TFT-Backlight (aktiv-low) | 15 |
| Touch CS / IRQ | 14 / 27 |
| LDR | 36 |

SPI verwendet die Standard-VSPI-Pins des ESP32: SCK 18, MISO 19, MOSI 23. Fritzing-Projekte und exportierte Bilder liegen in [`wiring/`](wiring/); ältere Zeichnungen können von dieser Pinbelegung abweichen.
