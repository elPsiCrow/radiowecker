// Pin map for the YelloByte YB-ESP32-S3-AMP (ESP32-S3-WROOM-1 N8R2). See README.md in this folder.
#pragma once

#define BOARD_NAME "YB-ESP32-S3-AMP"

// Both MAX98357A amplifiers share one I2S bus (left/right selected in hardware).
#define PIN_I2S_BCLK 5
#define PIN_I2S_LRCLK 6
#define PIN_I2S_DOUT 7

// microSD slot on the FSPI bus.
#define PIN_SD_CS 10
#define PIN_SD_MOSI 11
#define PIN_SD_SCK 12
#define PIN_SD_MISO 13

// Status LED; also mutes the amplifiers if solder bridge DAC_MUTE is closed (high = active).
#define PIN_STATUS_LED 47

// Optional user controls. Uncomment and set the GPIOs you wired; board_setup.cpp picks them up.
// The numbers below are examples only: check them against the board's pin layout and the strapping pins.
// Buttons connect the pin to GND (internal pull-up). See src/drivers/input/*/README.md.
// #define PIN_BTN_PLAY 4
// #define PIN_BTN_NEXT 15
// #define PIN_BTN_PREV 16
// #define PIN_BTN_VOL_UP 17
// #define PIN_BTN_VOL_DOWN 18
// #define PIN_ENCODER_A 8
// #define PIN_ENCODER_B 9
// #define PIN_ENCODER_SW 14  // Optional push switch of the encoder
