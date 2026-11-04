# Roadmap

Fork goal: turn the Arduino-IDE sketch into a modular PlatformIO C++ project that runs on
different hardware (display, input, audio, light) without touching the application logic.

## Decisions

| Topic | Decision |
| --- | --- |
| Build | PlatformIO, Arduino-ESP32 core 3.x (pioarduino stable channel), C++17 |
| Language | English identifiers and comments; UI strings stay translatable (i18n) |
| Upstream | Hard fork, no merges from `beabel/radiowecker` |
| UI contract | Core publishes a **view-model**; every display decides how to render it |
| Primary target | `yb_esp32_s3_amp` (YelloByte YB-ESP32-S3-AMP Rev.3, ESP32-S3-WROOM-1 N8R2) |
| Reference target | `esp32_ili9341` (original AZ-Touch hardware), compile-checked only |
| Test strategy | Serial display/input on the YB board; host-side unit tests for core logic |

## Target architecture

```text
src/
  main.cpp              startup, Arduino loop and coordination of app services
  app/                  controller, alarm scheduling, shared settings/state, board setup contract
  audio/                player (stream/SD source -> decoder -> output), alarm beep
  net/                  Wi-Fi, web server, weather, OTA
  ui/                   view-model, IDisplay, IInput, i18n, screen/page controller
  drivers/
    display/            serial/ | lvgl_ili9341/ | ssd1315/ | ipixel_ble/   (each with README.md)
    input/              serial/ | buttons/ | encoder/ | touch_xpt2046/
    outputs.h           IBacklight and ILightOutput contracts (implementation pending)
hardware/
  <board>/board_config.h  pins and optional controls selected per PlatformIO env
  <board>/board_setup.cpp register static-lifetime drivers for that board
  <board>/README.md       wiring, notes and hardware-specific docs
src/net/web_pages.h     embedded web UI (separate source assets are planned)
docs/                   build, flash, usage, architecture
test/                   host-side (native) unit tests
```

### Interfaces

- `IDisplay` is implemented: `begin()`, `render(const ViewModel&)`, `showStatus()`, optional `loop()` and capability reporting. Multiple displays can receive the same snapshot.
- `IInput` is implemented: non-blocking `poll()` produces semantic events (`PlayPause`, `Stop`, `Snooze`, station steps, volume changes and refresh). Multiple inputs can feed the same controller path.
- `IBacklight` is currently a contract for fixed or automatic brightness; no driver is registered yet.
- `ILightOutput` currently exposes `begin()` and `setLevel(0..1000)`. Sunrise scheduling/ramping is future application policy, not part of this driver interface yet.
- `IAudioSource` (`Stream(url)` / `File(path)` / `Beep`) is planned, not implemented. The current player is stream/MP3/I2S with an alarm beep path.

### Extension points kept open on purpose

- **Sunrise light:** alarm engine exposes `lightLeadMinutes`, `ILightOutput` implements the ramp.
- **SD ringtones:** `IAudioSource::File` and a storage driver; the board header already lists the SD pins.
- **BLE LED matrix:** needs NimBLE alongside Wi-Fi; the classic-ESP32 BT memory release in `setup()` becomes a build-flag decision.

## Task list

### Phase 0: Foundation

- [x] Roadmap
- [x] `platformio.ini` with YB Rev.3 and `esp32_ili9341` environments
- [x] Use the official pioarduino YB Rev.3 board definition (remove stale custom board JSON)
- [x] `hardware/<board>/board_config.h` for both boards
- [x] `.gitignore`, `.editorconfig`, `.clang-format`
- [x] Hello-world build of the S3 env to prove the toolchain

### Phase 1: Mechanical port (no behaviour change)

- [x] Move vendored/generated assets (fonts, `lv_conf.h`) to their module folders
- [x] Replace vendored `ArduinoJson.h` with a pinned `lib_deps` entry
- [x] Convert each `.ino` to `.cpp` with explicit headers and prototypes
- [x] Replace `00_settings.h` globals with `extern` declarations plus one definition file
- [x] Move `lv_conf.h` / `build_opt.h` into `build_flags`
- [x] `esp32_ili9341` env compiles; behavior is unverified on hardware
- [ ] Translate the remaining German comments (done so far: `main.cpp`, `alarm.cpp`)
- [ ] Replace deprecated `StaticJsonDocument` use in `webserver.cpp` / `http_update.cpp`
- [ ] Flash use is 86 % of the `no_fs` app slot on the classic ESP32; keep an eye on it

### Phase 2: Decouple core from hardware

- [ ] `AppState` / `Settings` structs replace loose globals (the controller now owns all state changes; the globals remain)
- [x] Alarm scheduling extracted as pure functions (unit-tested natively)
- [x] `ViewModel` and `IDisplay`, `IInput` defined and used by a `UiManager`; `IBacklight`, `ILightOutput` are headers only
- [x] LVGL screen code wrapped as the `lvgl_ili9341` display driver (it still reads shared globals)
- [x] Actions and settings moved out of the UI into `app/controller` with state-change listeners
- [ ] Logging wrapper replaces `RADIO_SERIAL`
- [ ] `IAudioSource` / `AudioOutput` abstraction around the player
- [ ] Remove hardcoded OTA password (`secrets.ini`)
- [ ] Translate remaining German comments in the other modules
- [ ] Replace the LVGL driver's direct global reads with the view-model

### Phase 3: New hardware

- [x] YB Rev.3 builds and has been smoke-tested on hardware: native USB serial, Wi-Fi streaming and I2S audio; web UI not separately verified
- [x] Serial display and serial input (primary test path)
- [x] GPIO buttons and rotary encoder input drivers with host-side tests; actual pins and physical controls remain unconfigured/unverified
- [ ] SSD1315 OLED display
- [ ] iPixel BLE matrix display (protocol research first)
- [ ] `ILightOutput` PWM driver and sunrise ramp
- [ ] SD storage driver and file audio source

### Phase 4: Web UI

- [ ] Split `src/net/web_pages.h` into `web/` files, per-language JSON
- [ ] Gzip and embed at build time (PlatformIO extra script)
- [ ] Self-host jQuery/FontAwesome or remove them (offline use)

### Phase 5: Docs and cleanup

- [x] Modern English/German PlatformIO README and I/O architecture documentation
- [x] Legacy Arduino-IDE and touch-screen setup guides retired
- [x] Legacy Fritzing files moved beside the `esp32_ili9341` reference hardware
- [ ] Review remaining screenshots and historical assets for relevance
- [x] README credits the upstream project lineage (Gerald Lechner, beabel)
- [ ] CI build for all environments
