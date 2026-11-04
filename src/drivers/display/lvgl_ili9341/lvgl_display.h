// IDisplay adapter for the original 320x240 ILI9341 + XPT2046 touch UI built with LVGL.
// The LVGL screens still read the shared application state directly; the view-model only drives updates.
#pragma once

#include "ui/display.h"

class LvglIli9341Display : public IDisplay {
 public:
  const char* name() const override { return "ILI9341 (LVGL)"; }
  DisplayCapabilities capabilities() const override;
  bool begin() override;
  void render(const ViewModel& vm) override;
  void showStatus(const StatusScreen& status) override;
  void loop() override;
  void onLanguageChanged() override;

 private:
  ViewModel last_;
  bool haveLast_ = false;
  bool statusShown_ = false;
  const char* otaTitle_ = nullptr;  // Title of the progress screen currently shown
};
