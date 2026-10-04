#include "app/app_state.h"

Preferences pref;
Preferences sender;

Station stationlist[STATIONS];

char ssid[CFG_SSID_MAX] = "";
char pkey[CFG_WPA_PSK_MAX] = "";
char ntp[CFG_NTP_HOST_MAX] = "de.pool.ntp.org";
float LATITUDE = 52.520645;
float LONGITUDE = 13.409779;
char TIME_ZONE_IANA[CFG_TZ_IANA_MAX] = "Europe/Berlin";

uint8_t curStation = 0;
uint8_t actStation = 0;
uint8_t curGain = 50;
uint8_t snoozeTime = 30;
uint8_t alarmSnoozeMin = 0;
uint8_t alarmGain = 50;
uint8_t alarmWakeMode = 0;
uint8_t bright = 25;
uint16_t alarm1 = 390;           // 06:30
uint16_t alarm2 = 480;           // 08:00
uint8_t alarmday1 = 0B00111110;  // Monday to Friday
uint8_t alarmday2 = 0B01000001;  // Saturday and Sunday

bool alarmBeepActive = false;
uint32_t lastchange = 0;
uint32_t snoozeTimeEnd = 0;
uint32_t alarmSnoozeUntil = 0;
bool alarmActionsVisible = false;
uint16_t alarmtime = 0;
uint8_t alarmday = 8;
char title[128];
bool newTitle = false;
uint32_t tick = 0;
uint32_t discon = 0;
uint16_t minutes;
uint8_t weekday;
struct tm ti;
int16_t lastldr;
uint32_t start_conf;
boolean connected;
boolean reconnected = false;
boolean radio = false;
boolean startpage = true;
boolean favoritenpage = false;
boolean settingspage = false;
boolean alarmpage = false;
boolean alarmgainpage = false;
unsigned long lastUpdate = 0;
uint16_t lastLedb = 0;
bool alarmTriggered = false;
unsigned long lastWeatherUpdate = 0;
