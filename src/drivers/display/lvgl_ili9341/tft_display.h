// Public interface of the LVGL + ILI9341 display driver. Phase 2 replaces this with IDisplay + a view-model.
#pragma once

#include <Arduino.h>

#include "app/app_config.h"
#include "net/weather.h"
#include "ui/ui_start_colors.h"

extern UiStartColors ui_start_cols;

// Setup and main loop hooks
void setup_display(void);
void touch_loop(void);  // Runs the LVGL timer handler
void setBGLight(uint8_t prct);

// Message and OTA screens (colors are RGB565; defaults are white on black)
void displayClear(void);
void displayMessage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, uint8_t align = ALIGNLEFT,
                    boolean big = false, uint16_t fc = 0xFFFF, uint16_t bg = 0x0000, uint8_t lines = 1);
void textInBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *text, uint8_t align, boolean big,
               uint16_t fc, uint16_t bg, uint8_t lines);
void ota_ui_begin(const char *title);
void ota_ui_set_sub(const char *sub);
void showProgress(uint32_t prc);

// Pages
void showStartPage(void);
void showFavoritenPage(void);
void showSettingsPage(void);
void showAlarmPage(void);
void showAlarmGainPage(void);
void showAlarms_Day_and_Time(void);
void safeAlarmTime(void);

// Start page content
void updateTime(boolean redraw);
void displayDateTime(void);
void displayWeather(const OpenMeteoTemps *t);
void showStation(void);
void showTitle(void);
void showRadio(void);
void showNextAlarm(void);
void updateStation(void);
void showStationList(void);
void drawHeaderInfos(void);
void drawWifiInfo(void);
void drawSnoozeInfo(void);
void FavoriteButtons(void);
void DrawFooterButtons_Power_Sleep_Alarm(void);
void tft_i18n_refresh_labels(void);

// Settings widgets
void showSlider(uint16_t y, float value, uint16_t vmax, uint16_t bgColor, uint16_t lineColor);
void showGain(void);
void showBrightness(void);
void showSnoozeTime(void);
void showAlarmSnoozeMin(void);
void ui_sync_all_gain_sliders(void);
void setGainValue(uint16_t value, const char *sliderPage);
void setBrightness(uint16_t value);
void setSnoozeTime(uint16_t value);

// Applied from the web UI
void web_apply_brightness(uint8_t v);
void web_apply_sleep_timer_min(uint8_t v);
void web_apply_alarm_snooze_min(uint8_t v);
void web_apply_alarm_gain(uint8_t v);
void web_apply_alarm_wake(uint8_t v);

// Start-page colors
void ui_start_colors_load_from_prefs(void);
void ui_start_colors_apply_from_web(const UiStartColors *src);
void ui_start_colors_reset_to_factory(void);

// Actions that currently live next to the UI code
void toggleRadio(boolean off, boolean keepAlarmSnoozePending = false, boolean useAlarmGainWhenStarting = false);
void alarm_action_stop(void);
void alarm_action_snooze(void);
void toggleAlarm(void);
void startSnooze(void);
void changeStation(void);
void selectStation(uint16_t x);
bool station_navigate_prev(void);
bool station_navigate_next(void);
bool station_navigate_select(uint8_t idx);

// Legacy no-op stubs kept for API parity with the pre-LVGL touch UI
void toggleAlarmDay(uint16_t xPos, uint16_t yPos);
void in_de_crementAlarmTimeHH(uint16_t xPos, uint16_t yPos);
void in_de_crementAlarmTimeMM(uint16_t xPos, uint16_t yPos);
void HandleFavoriteButtons(int xpos, int ypos);
