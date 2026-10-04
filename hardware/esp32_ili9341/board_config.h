// Pin map for the original hardware: ESP32 DevKit + ILI9341 TFT + XPT2046 touch + MAX98357A. See README.md.
#pragma once

#define BOARD_NAME "ESP32 + ILI9341"

#define PIN_I2S_LRCLK 25
#define PIN_I2S_BCLK 26
#define PIN_I2S_DOUT 33

#define PIN_TFT_CS 5
#define PIN_TFT_DC 4
#define PIN_TFT_RST 22
#define PIN_TFT_BACKLIGHT 15
#define TFT_BACKLIGHT_ACTIVE_LEVEL 0  // 0 = backlight is on when the pin is low

#define PIN_TOUCH_CS 14
#define PIN_TOUCH_IRQ 27

// Optional light-dependent resistor for automatic brightness.
#define PIN_LDR 36
