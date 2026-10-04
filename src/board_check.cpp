// Phase 0 toolchain check; replaced by the ported sketch in Phase 1.
#include <Arduino.h>

#include "board_config.h"

void setup() {
  Serial.begin(115200);
  Serial.printf("Radiowecker on %s, PSRAM: %u bytes\n", BOARD_NAME, (unsigned)ESP.getPsramSize());
}

void loop() { delay(1000); }
