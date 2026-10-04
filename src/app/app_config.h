// Compile-time configuration shared by all modules. Board pins live in hardware/<board>/board_config.h.
#pragma once

#include <stdint.h>

#define RADIOVERSION "v5.0.6"

#define STATIONS 30           // Number of station slots in the list
#define MINUTES_PER_DAY 1440  // 24 * 60

// Text alignment values used by displayMessage().
#define ALIGNLEFT 0
#define ALIGNCENTER 1
#define ALIGNRIGHT 2

// ESP8266Audio SetGain() range is 0..4; this is the effective maximum at 100 % on the volume slider.
#ifndef AUDIO_GAIN_MAX
#define AUDIO_GAIN_MAX 0.42f
#endif

// Network names
#define NETWORK_NAME "radioweckerlanname"  // Hostname on the local network
#define AP_NAME "radioweckerconf"          // SSID of the configuration soft-AP

// POSIX TZ string used for local time (Central Europe with DST).
#define TIME_ZONE "CET-1CEST,M3.5.0/02,M10.5.0/03"

// ArduinoOTA
#define OTA_HOSTNAME "radiowecker"
#define OTA_PASSWORD "weckerupdate"

// HTTP OTA from the web UI: the release asset must carry exactly this file name.
#define HTTP_OTA_GITHUB_OWNER "beabel"
#define HTTP_OTA_GITHUB_REPO "radiowecker"
#define HTTP_OTA_FIRMWARE_FILENAME "radiowecker-firmware.bin"
// Minimum free space in the inactive OTA slot (bytes); below this the update is aborted before download.
#define HTTP_OTA_MIN_FREE_BYTES 524288u
// NVS keys (namespace "radio"); key names are limited to 15 characters.
#define PREF_HTTP_OTA_PEND "http_otapend"
#define PREF_HTTP_OTA_TAG "http_otatag"

// Sizes of the char[] configuration buffers (avoids Arduino String heap fragmentation).
#define CFG_SSID_MAX 33      // 802.11 SSID: 32 characters + NUL
#define CFG_WPA_PSK_MAX 65   // WPA-PSK: 63 ASCII characters + NUL
#define CFG_NTP_HOST_MAX 96
#define CFG_TZ_IANA_MAX 64   // e.g. America/Argentina/ComodRivadavia

// Backlight tuning. In LDR mode (brightness 0 %) this is the minimum PWM value (0-255) so the screen stays usable.
#ifndef LDR_LED_PWM_MIN
#define LDR_LED_PWM_MIN 1
#endif

// LDR mode: weight of a new measurement is 1/DIV; larger values give slower, smoother changes.
#ifndef LDR_PWM_SMOOTH_DIV
#define LDR_PWM_SMOOTH_DIV 2
#endif

// Optional hardware buttons (to GND, INPUT_PULLUP); a pin below 0 means not connected.
// Stop switches radio/alarm off, or switches the radio on if nothing is running.
// Snooze is only evaluated while the alarm overlay is visible.
#ifndef ALARM_HW_BTN_STOP_PIN
#define ALARM_HW_BTN_STOP_PIN -1
#endif
#ifndef ALARM_HW_BTN_SNOOZE_PIN
#define ALARM_HW_BTN_SNOOZE_PIN -1
#endif

// Weather refresh interval in milliseconds.
constexpr unsigned long weatherUpdateInterval = 60000;
