// Serial diagnostics switch. RADIO_SERIAL(x) runs x only when RADIO_DEBUG_SERIAL is 1.
#pragma once

#include <Arduino.h>

// 0 removes all debug output (and Serial.begin) from the build. Override with -DRADIO_DEBUG_SERIAL=1.
#ifndef RADIO_DEBUG_SERIAL
#define RADIO_DEBUG_SERIAL 0
#endif

#if RADIO_DEBUG_SERIAL
#define RADIO_SERIAL(x) \
  do {                  \
    x;                  \
  } while (0)
#else
#define RADIO_SERIAL(x) ((void)0)
#endif
