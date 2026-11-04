#include "drivers/input/serial/serial_input.h"

#include <Arduino.h>
#include <ctype.h>
#include <stdlib.h>

namespace {

constexpr unsigned long kBaudRate = 115200;
constexpr int kVolumeStep = 5;

}  // namespace

bool SerialInput::begin() {
  Serial.begin(kBaudRate);
  return true;
}

bool SerialInput::poll(InputEvent& out) {
  while (Serial.available() > 0) {
    const char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (length_ == 0) continue;
      line_[length_] = '\0';
      length_ = 0;
      if (parse(line_, out)) return true;
      continue;
    }
    if (length_ < sizeof(line_) - 1) line_[length_++] = c;
  }
  return false;
}

bool SerialInput::parse(const char* line, InputEvent& out) {
  out.value = 0;
  switch (tolower((unsigned char)line[0])) {
    case 'p':
      out.type = InputEvent::Type::PlayPause;
      return true;
    case 's':
      out.type = InputEvent::Type::Stop;
      return true;
    case 'z':
      out.type = InputEvent::Type::Snooze;
      return true;
    case 'n':
      out.type = InputEvent::Type::NextStation;
      return true;
    case 'b':
      out.type = InputEvent::Type::PrevStation;
      return true;
    case '+':
      out.type = InputEvent::Type::VolumeDelta;
      out.value = kVolumeStep;
      return true;
    case '-':
      out.type = InputEvent::Type::VolumeDelta;
      out.value = -kVolumeStep;
      return true;
    case 'v':
      out.type = InputEvent::Type::VolumeSet;
      out.value = (int16_t)atoi(line + 1);
      return true;
    case 'i':
      out.type = InputEvent::Type::Refresh;
      return true;
    default:
      printHelp();
      return false;
  }
}

void SerialInput::printHelp() {
  Serial.println(
      "Commands (end with Enter): i show status, p play/pause, s stop alarm, z snooze or sleep timer, "
      "n next station, b previous station, + / - volume, v<0-100> set volume");
}
