// Contract between the application and an input device (buttons, encoder, touch, serial console).
#pragma once

#include <stdint.h>

// Semantic actions rather than raw key codes, so the same app logic works with any input hardware.
// Menu navigation events will be added together with the first menu-driven display.
struct InputEvent {
  enum class Type : uint8_t {
    PlayPause,
    Stop,
    Snooze,
    NextStation,
    PrevStation,
    VolumeDelta,  // value = signed change in percent points
    VolumeSet,    // value = 0-100
    Refresh,      // Show the current screen again
  };
  Type type;
  int16_t value = 0;
};

class IInput {
 public:
  virtual ~IInput() = default;

  virtual const char* name() const = 0;
  virtual bool begin() = 0;

  // Non-blocking. Returns true and fills `out` if an event is pending.
  virtual bool poll(InputEvent& out) = 0;
};
