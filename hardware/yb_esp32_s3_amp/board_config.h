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
