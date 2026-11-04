// Contract between the application and an output device that shows the UI.
#pragma once

#include <stdint.h>

#include "ui/view_model.h"

struct DisplayCapabilities {
  bool graphical = false;  // Pixel display; false for text-only outputs such as the serial console
  bool color = false;
  bool touch = false;
  uint16_t width = 0;   // Pixels or character columns
  uint16_t height = 0;  // Pixels or character rows
};

class IDisplay {
 public:
  virtual ~IDisplay() = default;

  virtual const char* name() const = 0;
  virtual DisplayCapabilities capabilities() const = 0;

  // Returns false if the hardware is missing or failed to initialise; the app then drops this display.
  virtual bool begin() = 0;

  // Called whenever the view-model revision changed and at least once per second for the clock.
  virtual void render(const ViewModel& vm) = 0;

  // Replaces the normal UI until the app calls render() again.
  virtual void showStatus(const StatusScreen& status) = 0;

  // The UI language changed; drivers that cache translated text must refresh it.
  virtual void onLanguageChanged() {}

  // The user asked to see the current screen again. Matters for outputs that do not keep their content,
  // such as a serial console opened after the device booted.
  virtual void redraw() {}

  // Called from the main loop for drivers that need regular servicing (UI timers, scrolling, animation).
  virtual void loop() {}
};
