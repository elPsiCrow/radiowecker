# Radiowecker

Ein modular aufgebautes Internet-Radio mit Wecker für ESP32, gebaut als C++-Projekt mit PlatformIO. Dieses Repository ist ein hardwareflexibler Fork des [ursprünglichen Radiowecker-Projekts](https://github.com/beabel/radiowecker), das auf Gerald Lechners AZ-Delivery-Entwurf zurückgeht.

Das erste Entwicklungssystem ist das **YelloByte YB-ESP32-S3-AMP Rev.3**. Es läuft derzeit mit einer seriellen Konsolenoberfläche und den beiden MAX98357A-Verstärkern. Die ursprüngliche ESP32-Konfiguration mit ILI9341 und XPT2046 bleibt als Referenz zum Kompilieren erhalten; in diesem Fork wurde sie nicht auf echter Hardware getestet.

English version: [README.md](README.md).

## Schnellstart

Benötigt werden VS Code mit PlatformIO-Erweiterung, ein USB-Anschluss zum Board und die in `platformio.ini` festgelegte PlatformIO-Plattform und Bibliotheken. Unter Windows müssen lange Pfade aktiviert sein, da das ESP32-Core-Paket Pfade mit mehr als 260 Zeichen enthält.

Im PlatformIO-Terminal:

```powershell
pio run -e yb_esp32_s3_amp
pio run -e yb_esp32_s3_amp -t upload
pio device monitor -e yb_esp32_s3_amp
```

Die Board-Konfiguration ist für Rev.3 mit nativem USB-CDC-Serial ausgelegt. Falls PlatformIO den Port nicht automatisch findet, wähle ihn im Monitor aus. Die Baudrate ist 115200. Drücke nach dem Öffnen des Monitors die Reset-Taste `R`, um das vollständige Boot-Log zu sehen; mit `i` und Enter wird der aktuelle Status erneut ausgegeben.

Ohne gespeicherte WLAN-Zugangsdaten startet das Gerät den Access Point `radioweckerconf`. Verbinde dich damit und öffne <http://192.168.4.1>, um das WLAN einzurichten. Danach zeigt die serielle Konsole Uhrzeit, Wiedergabe, Lautstärke, Wecker und Netzwerkstatus.

| Befehl | Funktion |
| --- | --- |
| `i` | Status erneut ausgeben |
| `p` | Wiedergabe starten/anhalten |
| `s` | Wiedergabe oder Weckton beenden |
| `z` | Wecker schlummern lassen; sonst Einschlaftimer starten |
| `n` / `b` | Nächsten / vorherigen aktiven Sender wählen |
| `+` / `-` | Lautstärke um 5 ändern |
| `v40` | Lautstärke auf 40 Prozent setzen |

Für die Hardware-Inbetriebnahme sind im YB-Build derzeit ausführliche Anwendungs- und Arduino-Core-Logs aktiviert. Für den Dauerbetrieb können `RADIO_DEBUG_SERIAL` und `CORE_DEBUG_LEVEL` in `platformio.ini` reduziert werden.

## Bauen und Testen

```powershell
pio run -e yb_esp32_s3_amp
pio run -e esp32_ili9341
pio test -e native
```

`esp32_ili9341` prüft, ob die Referenzkonfiguration kompiliert. Ohne das ursprüngliche Board und Display kann ihr Verhalten nicht geprüft werden. Die nativen Tests decken Alarmplanung, Tasten-Entprellung/Wiederholung und Drehencoder-Decodierung ab. Sie benötigen einen C++-Compiler auf dem Entwicklungsrechner (unter Windows z. B. GCC).

## Hardware

- [YB-ESP32-S3-AMP Rev.3](hardware/yb_esp32_s3_amp/README.md): Hauptboard, I2S-Verstärker und SD-Sockel, serielle Inbetriebnahme und optionale Bedienelemente.
- [ESP32 + ILI9341](hardware/esp32_ili9341/README.md): Referenzkonfiguration mit TFT/Touch und erhaltenen Schaltplänen.

Die GPIO-Beispiele in `board_config.h` sind keine Anschluss-Empfehlung. Prüfe Board-Revision, Pinbelegung und Boot-Strapping-Pins, bevor Bedienelemente angeschlossen werden.

## Projektstruktur

```text
src/app/        Anwendungsaktionen, Alarmplanung, Zustand und Board-Setup-Vertrag
src/audio/      Internet-Stream und I2S-Wiedergabe
src/net/        WLAN, Web-API, Wetter und OTA
src/ui/         View-Model, Eingabe-/Ausgabe-Verträge und UI-Manager
src/drivers/    Serielle und LVGL-Display-/Eingabetreiber, künftige Hardwaretreiber
hardware/       Board-Pins, Registrierung der Treiber und Hardware-Dokumentation
test/           Native Unit-Tests mit PlatformIO
docs/           Architektur und Projekt-Roadmap
```

Die [Architekturdokumentation](docs/ARCHITECTURE.de.md) erklärt die I/O-Verträge und ihre aktuellen Grenzen. Die [Roadmap](docs/ROADMAP.md) führt geplante Hardware und Aufräumarbeiten auf. Treiberspezifische Hinweise liegen direkt neben dem jeweiligen Code.

## Herkunft

Der Radiowecker-Entwurf und Teile der Firmware gehen auf Gerald Lechners AZ-Delivery-Projekt und das Repository `beabel/radiowecker` zurück. Dieser Fork wird unabhängig weiterentwickelt. Die ursprüngliche Namensnennung und die Lizenzen der verwendeten Drittanbieter-Bibliotheken bleiben gültig; maßgeblich sind die jeweiligen Projekt- und Komponentenlizenzen.
