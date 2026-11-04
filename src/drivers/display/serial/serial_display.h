// Text-only display that prints status changes to the serial console. Works on any board.
#pragma once

#include "ui/display.h"

class SerialDisplay : public IDisplay {
 public:
  const char* name() const override { return "Serial console"; }
  DisplayCapabilities capabilities() const override;
  bool begin() override;
  void render(const ViewModel& vm) override;
  void showStatus(const StatusScreen& status) override;
  void redraw() override;

 private:
  char lastLine_[256] = "";
  char lastStatus_[320] = "";  // Text of the most recent status screen
  bool statusShown_ = false;
  int lastProgress_ = -1;
};
