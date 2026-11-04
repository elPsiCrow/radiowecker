// Reads one-line commands from the serial console and turns them into input events.
#pragma once

#include "ui/input.h"

class SerialInput : public IInput {
 public:
  const char* name() const override { return "Serial console"; }
  bool begin() override;
  bool poll(InputEvent& out) override;

 private:
  bool parse(const char* line, InputEvent& out);
  static void printHelp();

  char line_[16] = "";
  uint8_t length_ = 0;
};
