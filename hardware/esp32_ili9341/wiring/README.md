# Legacy Reference Wiring

These Fritzing projects and exported diagrams describe the original ESP32 DevKit + ILI9341/XPT2046 build. They are retained as hardware references; this setup is not the primary development target and has not been tested in this fork.

- [`cheap/`](cheap/): breadboard build (`cheap_radiowecker.fzz`) and diagrams.
- [`min/`](min/): compact build (`min_radiowecker.fzz`) and diagrams.
- [`original_pinout.png`](original_pinout.png): legacy pinout sketch.

The active pin map for the PlatformIO reference environment is in [`../board_config.h`](../board_config.h). Verify all wiring against that file before using these legacy drawings; upstream diagrams may describe an earlier revision.
