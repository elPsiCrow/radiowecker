#include "app/board_setup.h"

#include "drivers/display/lvgl_ili9341/lvgl_display.h"
#include "ui/ui_manager.h"

void boardSetup(UiManager& ui) {
  static LvglIli9341Display display;
  ui.addDisplay(&display);
}
