# Radiowecker

Deutsch: [README.de.md](README.de.md).

A modular, PlatformIO-based C++ internet radio and alarm clock for ESP32. This is a hardware-flexible fork of the [original Radiowecker project](https://github.com/beabel/radiowecker), which grew from Gerald Lechner's AZ-Delivery design.

The first supported development target is the **YelloByte YB-ESP32-S3-AMP Rev.3**. It currently runs through a serial-console UI and the board's two MAX98357A amplifiers. The original ESP32 + ILI9341 + XPT2046 configuration remains as a compile-checked reference; it has not been tested on hardware in this fork.

## Quick Start

Requirements: VS Code with the PlatformIO extension, USB connection to the board, and the PlatformIO platform/libraries specified in `platformio.ini`. On Windows, enable long paths before the first build; the ESP32 core package contains paths that exceed the legacy 260-character limit.

In the PlatformIO terminal:

```powershell
pio run -e yb_esp32_s3_amp
pio run -e yb_esp32_s3_amp -t upload
pio device monitor -e yb_esp32_s3_amp
```

The board definition is for YB-ESP32-S3-AMP Rev.3, which exposes native USB CDC serial. Select its serial port in PlatformIO if automatic detection does not find it. The monitor baud rate is 115200. Press the board's `R` button after opening the monitor to see the complete boot log; type `i` and press Enter to print the current status again.

Without saved Wi-Fi credentials, the device starts the `radioweckerconf` access point. Join it and open <http://192.168.4.1> to configure Wi-Fi. Once connected, the serial console shows the clock, playback, volume, alarm and network state.

| Serial command | Action |
| --- | --- |
| `i` | Print current status |
| `p` | Play / pause |
| `s` | Stop playback or alarm |
| `z` | Snooze an active alarm; otherwise start the sleep timer |
| `n` / `b` | Next / previous enabled station |
| `+` / `-` | Volume up / down by 5 |
| `v40` | Set volume to 40 percent |

The YB environment currently enables detailed application and Arduino-core logging for hardware bring-up. Reduce `RADIO_DEBUG_SERIAL` and `CORE_DEBUG_LEVEL` in `platformio.ini` for normal operation.

## Build And Test

```powershell
pio run -e yb_esp32_s3_amp
pio run -e esp32_ili9341
pio test -e native
```

`esp32_ili9341` verifies the original TFT build, but hardware behavior cannot be checked without that board and display. Native tests cover alarm scheduling, button debounce/repeat and rotary-encoder decoding. They require a host C++ compiler (GCC on Windows).

## Hardware

- [YB-ESP32-S3-AMP Rev.3](hardware/yb_esp32_s3_amp/README.md): primary board, onboard I2S amplifiers and SD socket, serial bring-up, configurable optional controls.
- [ESP32 + ILI9341](hardware/esp32_ili9341/README.md): original TFT/touch reference and preserved wiring diagrams.

GPIO examples in `board_config.h` are not a pin assignment recommendation. Check the exact board revision, pin layout and ESP32-S3 strapping pins before connecting controls.

## Project Map

```text
src/app/        Application actions, alarm scheduling, state and board setup contract
src/audio/      Internet stream and I2S playback
src/net/        Wi-Fi, web API, weather and OTA
src/ui/         Input/output contracts, view-model and UI manager
src/drivers/    Serial and LVGL display/input drivers; future hardware drivers
hardware/       Board-specific pins, driver registration and wiring notes
test/           PlatformIO native unit tests
docs/           Architecture and implementation roadmap
```

See [Architecture](docs/ARCHITECTURE.md) for the I/O contracts and current coupling, and the [Roadmap](docs/ROADMAP.md) for planned hardware and cleanup work. Driver-specific controls and wiring notes live next to their implementations.

## Project Lineage

The alarm-radio design and parts of its firmware derive from Gerald Lechner's AZ-Delivery project and the `beabel/radiowecker` repository. This fork is being restructured independently; original attribution and third-party library licenses remain applicable. See the upstream repository and component licenses for their respective terms.
