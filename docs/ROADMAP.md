# Roadmap

Fork goal: turn the Arduino-IDE sketch into a modular PlatformIO C++ project that runs on
different hardware (display, input, audio, light) without touching the application logic.

## Decisions

| Topic | Decision |
|---|---|
| Build | PlatformIO, Arduino-ESP32 core 3.x (via the pioarduino platform), C++17 |
| Language | English identifiers and comments; UI strings stay translatable (i18n) |
| Upstream | Hard fork, no merges from `beabel/radiowecker` |
| UI contract | Core publishes a **view-model**; every display decides how to render it |
| Primary target | `yb_esp32_s3_amp` (YelloByte board, ESP32-S3-WROOM-1 N8R2) |
| Reference target | `esp32_ili9341` (original AZ-Touch hardware), compile-checked only |
| Test strategy | Serial display/input on the YB board; host-side unit tests for core logic |

## Target architecture

```
src/
  main.cpp              wiring only: pick drivers from build flags, run the app loop
  app/                  platform-light core: settings, clock, alarms, stations, app state
  audio/                player (stream/SD source -> decoder -> output), alarm beep
  net/                  Wi-Fi, web server, weather, OTA
  ui/                   view-model, IDisplay, IInput, i18n, screen/page controller
  drivers/
    display/            serial/ | lvgl_ili9341/ | ssd1315/ | ipixel_ble/   (each with README.md)
    input/              serial/ | buttons/ | encoder/ | touch_xpt2046/
    light/              pwm/            (ILightOutput, used for sunrise-style alarm dimming)
    storage/            sd/             (IAudioSource implementation for ringtones)
hardware/
  <board>/board_config.h  pins + capabilities, selected per PlatformIO env
  <board>/README.md       wiring, notes, schematics (Fritzing files move here)
web/                    html/js/css/i18n json, embedded into the firmware at build time
docs/                   build, flash, usage, architecture
test/                   host-side (native) unit tests
```

### Interfaces (Phase 2)

- `IDisplay`: `begin()`, `render(const ViewModel&)`, optional `tick()`; capabilities flag (text-only, mono, color, touch).
- `IInput`: `poll()` produces `InputEvent` values (Up, Down, Select, Back, Stop, Snooze, VolumeDelta, ...).
- `IBacklight`: brightness 0-100 (PWM, LDR-driven, or none).
- `ILightOutput`: level 0-1000 and ramp request; the alarm engine asks for a ramp N minutes before the alarm.
- `IAudioSource`: `Stream(url)` | `File(path)` | `Beep`; the player does not care where audio comes from (SD ringtones).
- Multiple displays and inputs may be active at once (e.g. serial + OLED), so the app holds a list of each.

### Extension points kept open on purpose

- **Sunrise light:** alarm engine exposes `lightLeadMinutes`, `ILightOutput` implements the ramp.
- **SD ringtones:** `IAudioSource::File` and a storage driver; the board header already lists the SD pins.
- **BLE LED matrix:** needs NimBLE alongside Wi-Fi; the classic-ESP32 BT memory release in `setup()` becomes a build-flag decision.

## Task list

### Phase 0: Foundation
- [x] Roadmap
- [x] `platformio.ini` with `yb_esp32_s3_amp` and `esp32_ili9341` environments, pinned platform and libraries
- [x] Custom board JSON for the YB board (`boards/`)
- [x] `hardware/<board>/board_config.h` for both boards
- [x] `.gitignore`, `.editorconfig`, `.clang-format`
- [x] Hello-world build of the S3 env to prove the toolchain

### Phase 1: Mechanical port (no behaviour change)
- [x] Move vendored/generated assets (fonts, `lv_conf.h`) to their module folders
- [x] Replace vendored `ArduinoJson.h` with a pinned `lib_deps` entry
- [x] Convert each `.ino` to `.cpp` with explicit headers and prototypes
- [x] Replace `00_settings.h` globals with `extern` declarations plus one definition file
- [x] Move `lv_conf.h` / `build_opt.h` into `build_flags`
- [x] `esp32_ili9341` env compiles; behaviour reviewed by reading, not by running
- [ ] Translate the remaining German comments (done so far: `main.cpp`, `alarm.cpp`)
- [ ] Replace deprecated `StaticJsonDocument` use in `webserver.cpp` / `http_update.cpp`
- [ ] Flash use is 86 % of the `no_fs` app slot on the classic ESP32; keep an eye on it

### Phase 2: Decouple core from hardware
- [ ] `AppState` / `Settings` structs replace loose globals
- [ ] Alarm scheduling extracted as pure functions (unit-tested natively)
- [ ] `ViewModel` and `IDisplay`, `IInput`, `IBacklight` defined
- [ ] LVGL screen code wrapped as the `lvgl_ili9341` display driver
- [ ] Logging wrapper replaces `RADIO_SERIAL`
- [ ] `IAudioSource` / `AudioOutput` abstraction around the player
- [ ] Remove hardcoded OTA password (`secrets.ini`)

### Phase 3: New hardware
- [ ] YB board env: I2S on GPIO 5/6/7, PSRAM, USB serial settings, S3-safe startup
- [ ] Serial display and serial input (primary test path)
- [ ] GPIO buttons input, then rotary encoder and potentiometer
- [ ] SSD1315 OLED display
- [ ] iPixel BLE matrix display (protocol research first)
- [ ] `ILightOutput` PWM driver and sunrise ramp
- [ ] SD storage driver and file audio source

### Phase 4: Web UI
- [ ] Split `index.h` into `web/` files, per-language JSON
- [ ] Gzip and embed at build time (PlatformIO extra script)
- [ ] Self-host jQuery/FontAwesome or remove them (offline use)

### Phase 5: Docs and cleanup
- [ ] Hardware docs and Fritzing files moved to `hardware/<board>/`
- [ ] Arduino-IDE docs rewritten for PlatformIO, in English and German
- [ ] README rewritten; upstream authors credited (Gerald Lechner, beabel)
- [ ] CI build for all environments
