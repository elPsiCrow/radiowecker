// Station list: defaults, NVS persistence and reordering.
#pragma once

#include <stdint.h>

typedef struct {
  char url[150];    // Stream URL
  char name[32];    // Display name
  uint8_t enabled;  // 1 = shown in the player, 0 = hidden
} Station;

void setup_senderList();  // Load from NVS, falling back to the built-in defaults
void restore();           // Reset to the built-in defaults and persist them
void saveList();
void reorder(uint8_t oldpos, uint8_t newpos);
