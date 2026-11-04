# YB-ESP32-S3-AMP Rev.3

Entwicklungsboard von yellobyte mit ESP32-S3-WROOM-1 (N8R2: 8 MB Flash, 2 MB QSPI-PSRAM), zwei MAX98357A-Verstärkern, microSD-Steckplatz und nativem USB-Serial/JTAG. [Board-Dokumentation](https://github.com/yellobyte/YB-ESP32-S3-AMP).

PlatformIO-Umgebung: `yb_esp32_s3_amp` (`board = yb_esp32s3_amp_v3`). Englische Version: [README.md](README.md).

## Fest belegte Pins

| Funktion | GPIO | Hinweis |
| --- | ---: | --- |
| I2S BCLK | 5 | Nicht auf Header geführt |
| I2S LRCLK | 6 | Nicht auf Header geführt |
| I2S DOUT | 7 | Nicht auf Header geführt |
| SD CS / MOSI / SCK / MISO | 10 / 11 / 12 / 13 | FSPI; 11-13 nicht auf Header geführt |
| Status-LED | 47 | Bei geschlossenem Lötjumper `DAC_MUTE` auch Verstärker-Stummschaltung |
| Boot-Taste | 0 | |

## Serielle Ausgabe

Rev.3 verwendet natives ESP32-S3-USB-CDC für `Serial`. Beim Reset kann sich der Monitor trennen und erneut verbinden; das ist normal. Die Firmware wartet beim Start bis zu zwei Sekunden auf den Monitor. Falls Upload nicht automatisch startet: `B` gedrückt halten, `R` kurz drücken, dann `B` loslassen.

## Erster Start

1. `pio run -e yb_esp32_s3_amp -t upload`, anschließend `pio device monitor -e yb_esp32_s3_amp` öffnen (115200 Baud).
2. Ohne gespeicherte Zugangsdaten startet der Access Point `radioweckerconf`. Verbinden und <http://192.168.4.1> öffnen, um WLAN einzurichten.
3. Nach dem Neustart zeigt die Konsole Statusänderungen an. Mit `p` starten, mit `n` den Sender wechseln und mit `v40` die Lautstärke auf 40 Prozent setzen. Weitere Befehle stehen unter [serieller Eingabe](../../src/drivers/input/serial/README.md).

Für andere WROOM-Varianten können Flash- und PSRAM-Einstellungen abweichen. Vor dem Flashen die passende offizielle Board-Umgebung auswählen und deren Build-Einstellungen prüfen.
