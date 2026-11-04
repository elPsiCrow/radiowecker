// Application entry point: boot sequence and main loop.
#include <ArduinoOTA.h>
#include <WiFi.h>

#include "app/alarm.h"
#include "app/app_config.h"
#include "app/app_state.h"
#include "app/board_setup.h"
#include "app/controller.h"
#include "app/log.h"
#include "audio/audio.h"
#include "board_config.h"
#include "net/http_update.h"
#include "net/ota.h"
#include "net/weather.h"
#include "net/webserver.h"
#include "net/wlan.h"
#include "ui/i18n.h"
#include "ui/ui_manager.h"

// Classic ESP32 only: release Bluetooth RAM for the application. Declared here to avoid pulling in esp_bt.h.
#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ESP32) && CONFIG_IDF_TARGET_ESP32
extern "C" int esp_bt_controller_mem_release(int mode);
#ifndef ESP_BT_MODE_BTDM
#define ESP_BT_MODE_BTDM 3
#endif
#endif

static void setup_alarm_hw_buttons(void);
static void poll_alarm_hw_buttons(void);
static void alarm_hw_stop_button_action(void);

void setup() {
  RADIO_SERIAL(Serial.begin(115200); delay(100); Serial.println("Serial initialized"););
#if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_IDF_TARGET_ESP32) && CONFIG_IDF_TARGET_ESP32
  esp_bt_controller_mem_release(ESP_BT_MODE_BTDM);
#endif
  RADIO_SERIAL(Serial.println("Load preferences"));

  title[0] = 0;

  // Two NVS namespaces: "radio" for settings, "senderlist" for the station list.
  pref.begin("radio", false);
  i18n_begin();

  sender.begin("senderlist", false);

  if (pref.isKey("ssid")) pref.getString("ssid", ssid, sizeof(ssid));
  if (pref.isKey("pkey")) pref.getString("pkey", pkey, sizeof(pkey));
  if (pref.isKey("ntp")) pref.getString("ntp", ntp, sizeof(ntp));
  if (pref.isKey("TIME_ZONE_IANA")) pref.getString("TIME_ZONE_IANA", TIME_ZONE_IANA, sizeof(TIME_ZONE_IANA));
  if (pref.isKey("LATITUDE")) LATITUDE = pref.getFloat("LATITUDE");
  if (pref.isKey("LONGITUDE")) LONGITUDE = pref.getFloat("LONGITUDE");

  curGain = 50;
  if (pref.isKey("gain")) curGain = (uint8_t)constrain((int)pref.getUShort("gain"), 0, 100);

  snoozeTime = 30;  // Sleep timer default in minutes
  if (pref.isKey("snooze")) snoozeTime = (uint8_t)constrain((int)pref.getUShort("snooze"), 0, 60);

  alarmSnoozeMin = 0;
  if (pref.isKey("alm_snooze")) alarmSnoozeMin = (uint8_t)constrain((int)pref.getUShort("alm_snooze"), 0, 10);

  alarmGain = 50;
  if (pref.isKey("alm_gain")) alarmGain = (uint8_t)constrain((int)pref.getUShort("alm_gain"), 0, 100);

  alarmWakeMode = 0;
  if (pref.isKey("alm_wake")) alarmWakeMode = (uint8_t)(pref.getUChar("alm_wake") & 1u);

  bright = 80;
  if (pref.isKey("bright")) bright = pref.getUShort("bright");

  alarm1 = 390;  // 06:30
  if (pref.isKey("alarm1")) alarm1 = pref.getUInt("alarm1");

  alarmday1 = 0B00111110;  // Monday to Friday
  if (pref.isKey("alarmday1")) alarmday1 = pref.getUShort("alarmday1");

  alarm2 = 480;  // 08:00
  if (pref.isKey("alarm2")) alarm2 = pref.getUInt("alarm2");

  alarmday2 = 0B01000001;  // Saturday and Sunday
  if (pref.isKey("alarmday2")) alarmday2 = pref.getUShort("alarmday2");

  alarmtime = 0;
  alarmday = 8;  // 8 = no alarm scheduled

  curStation = 0;
  if (pref.isKey("station")) curStation = pref.getUShort("station");
  if (curStation >= STATIONS) curStation = 0;

  actStation = curStation;

  RADIO_SERIAL(Serial.printf("station %i, gain %i, ssid %s, ntp %s\n", curStation, curGain, ssid, ntp));
  RADIO_SERIAL(Serial.printf_P(PSTR("heap before setup_audio: %u\n"), (unsigned)ESP.getFreeHeap()));

  setup_audio();
  RADIO_SERIAL(Serial.printf_P(PSTR("heap after setup_audio: %u\n"), (unsigned)ESP.getFreeHeap()));
  RADIO_SERIAL(Serial.println(F("ui.begin...")));
  boardSetup(ui);
  ui.begin();
  RADIO_SERIAL(Serial.printf_P(PSTR("heap after ui.begin: %u\n"), (unsigned)ESP.getFreeHeap()));

  // HTTP OTA continues after the restart that the web UI triggered; it reuses the OTA screen.
  if (pref.getUChar(PREF_HTTP_OTA_PEND, 0) != 0) {
    httpOtaRunDeferredBoot();
  }

  // Audio buffers are allocated on the first startUrl(); allocating here costs ~70 KiB of heap that
  // LwIP, weather and TCP need.
  RADIO_SERIAL(Serial.printf_P(PSTR("heap before setup_senderList: %u\n"), (unsigned)ESP.getFreeHeap()));
  setup_senderList();
  RADIO_SERIAL(Serial.printf_P(PSTR("heap after setup_senderList: %u\n"), (unsigned)ESP.getFreeHeap()));
  setGain(curGain);

  {
    StatusScreen connecting;
    connecting.title = i18n_str(I18N_CONNECTING_TO);
    connecting.lines[0] = ssid;
    connecting.lineCount = 1;
    ui.showStatus(connecting);
  }
  RADIO_SERIAL(Serial.println("Connect WiFi"));

  connected = initWiFi(ssid, pkey);

  if (connected) {
    configTzTime(TIME_ZONE, ntp);

    delay(500);  // Give SNTP a moment before the first read
    getLocalTime(&ti);
    minutes = ti.tm_hour * 60 + ti.tm_min;
    weekday = ti.tm_wday;

    RADIO_SERIAL(Serial.println("Start"));

    if (pref.isKey("alarmon") && pref.getBool("alarmon")) findNextAlarm();
  } else {
    StatusScreen notConnected;
    notConnected.tone = StatusTone::Error;
    notConnected.title = i18n_str(I18N_NOT_CONNECTED);
    notConnected.lines[0] = i18n_str(I18N_CONNECT_TO_AP);
    notConnected.lines[1] = AP_NAME;
    notConnected.lines[2] = i18n_str(I18N_CONFIG_IP);
    notConnected.lines[3] = "192.168.4.1";
    notConnected.lineCount = 4;
    ui.showStatus(notConnected);

    discon = millis();  // Start of the 5-minute window after which loop() restarts the device
  }

  // Start the web server before the start page is built: less heap fragmentation at WebServer::begin().
  RADIO_SERIAL(Serial.println("Start webserver"));
  setup_webserver();
  setup_ota();

  if (connected) {
    ui.showHome();
  }

  setup_alarm_hw_buttons();

  start_conf = 0;
}

void loop() {
  ArduinoOTA.handle();

  // Audio runs before web server and LVGL to avoid buffer underruns under Wi-Fi and UI load.
  if (connected && (radio || decoder || alarmBeepActive)) {
    audio_loop();
  }

  webserver_loop();

  ui.loop();

  poll_alarm_hw_buttons();

  // Feed the audio pipeline again: a screen change can keep the display busy long enough to underrun.
  if (connected && (radio || decoder || alarmBeepActive)) {
    audio_loop();
  }

  if (connected && (WiFi.status() != WL_CONNECTED)) {
    connected = false;
    discon = millis();
    StatusScreen lost;
    lost.tone = StatusTone::Error;
    lost.title = i18n_str(I18N_NOT_CONNECTED);
    ui.showStatus(lost);
  }

  if (!connected && (WiFi.status() == WL_CONNECTED)) {
    connected = true;
    lastWeatherUpdate = 0;  // Fetch the weather soon after reconnecting instead of waiting for the interval
    StatusScreen back;
    back.tone = StatusTone::Ok;
    back.title = i18n_str(I18N_RECONNECTED);
    ui.showStatus(back);
    reconnected = true;  // The status screen is replaced by the home screen on the next second tick
  }

  // Only with a station connection: in AP-only mode there is no internet.
  if (millis() - lastWeatherUpdate > weatherUpdateInterval || lastWeatherUpdate == 0) {
    if (connected && !radio && startpage) {
      OpenMeteoTemps wt;
      if (fetchOpenMeteoWeather(&wt, LATITUDE, LONGITUDE, TIME_ZONE_IANA)) {
        weatherState.valid = true;
        weatherState.revision++;
        weatherState.currentC = wt.current_c;
        weatherState.feelsC = wt.feels_c;
        weatherState.todayMinC = wt.today_min;
        weatherState.todayMaxC = wt.today_max;
        weatherState.tomorrowMinC = wt.tomorrow_min;
        weatherState.tomorrowMaxC = wt.tomorrow_max;
        ui.markDirty();
      }
    }
    lastWeatherUpdate = millis();
  }

  // Once-per-second tasks
  if ((millis() - lastUpdate) > 1000) {
    lastUpdate = millis();

    // Sleep timer elapsed: radio off.
    if (millis() > snoozeTimeEnd && snoozeTimeEnd > 0) {
      snoozeTimeEnd = 0;
      playbackStop();
    }

    // Alarm snooze elapsed: start the wake-up sound again.
    if (alarmSnoozeUntil != 0 && millis() >= alarmSnoozeUntil) {
      alarmSnoozeUntil = 0;
      alarmActionsVisible = true;
      if (alarmWakeMode == 1) {
        alarm_beep_start();
      } else {
        playbackStart(true);
      }
      ui.markDirty();  // The beep path does not go through the controller
    }

    if (connected && getLocalTime(&ti)) {
      minutes = ti.tm_hour * 60 + ti.tm_min;
      weekday = ti.tm_wday;
    }

    if (connected && reconnected) {
      reconnected = false;
      ui.showHome();
    }

    // Alarm check, only when the alarm is switched on in NVS.
    if (pref.isKey("alarmon") && pref.getBool("alarmon") && (alarmday < 8) && getLocalTime(&ti)) {
      if ((alarmday == weekday) && (minutes == alarmtime) && !alarmTriggered) {
        alarmTriggered = true;  // Fire only once per alarm minute
        alarmActionsVisible = true;
        if (alarmWakeMode == 1) {
          alarm_beep_start();
        } else {
          playbackStart(true);  // Radio on at alarm volume
        }
        findNextAlarm();
        ui.markDirty();
      }
    }
  }

  // Re-arm once the clock has left the alarm minute.
  if (minutes != alarmtime) {
    alarmTriggered = false;
  }

  // Restart after more than 5 minutes without a connection.
  if (!connected && ((millis() - discon) > 300000)) ESP.restart();
}

static void setup_alarm_hw_buttons(void) {
  if (ALARM_HW_BTN_STOP_PIN >= 0) pinMode((uint8_t)ALARM_HW_BTN_STOP_PIN, INPUT_PULLUP);
  if (ALARM_HW_BTN_SNOOZE_PIN >= 0) pinMode((uint8_t)ALARM_HW_BTN_SNOOZE_PIN, INPUT_PULLUP);
}

static void poll_alarm_hw_buttons(void) {
  if (ALARM_HW_BTN_STOP_PIN < 0 && ALARM_HW_BTN_SNOOZE_PIN < 0) return;

  if (ALARM_HW_BTN_STOP_PIN >= 0) {
    static uint8_t stop_wait_release;
    int lv = digitalRead(ALARM_HW_BTN_STOP_PIN);
    if (stop_wait_release == 0) {
      if (lv == LOW) {
        alarm_hw_stop_button_action();
        stop_wait_release = 1;
      }
    } else if (lv != LOW) {
      stop_wait_release = 0;
    }
  }

  if (ALARM_HW_BTN_SNOOZE_PIN >= 0) {
    static uint8_t snooze_wait_release;
    int lv = digitalRead(ALARM_HW_BTN_SNOOZE_PIN);
    if (snooze_wait_release == 0) {
      if (lv == LOW) {
        if (alarmActionsVisible) {
          alarmSnooze();
          snooze_wait_release = 1;
        } else {
          snooze_wait_release = 2;  // Ignore this press until the button is released
        }
      }
    } else if (lv != LOW) {
      snooze_wait_release = 0;
    }
  }
}

// Hardware stop: switch everything off; if nothing is running, switch the radio on instead.
static void alarm_hw_stop_button_action(void) {
  if (radio || alarmActionsVisible || alarmSnoozeUntil != 0) {
    playbackStop();
  } else {
    playbackStart();
  }
}
