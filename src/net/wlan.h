#pragma once

#include <Arduino.h>

boolean initWiFi(const char *wifiSsid, const char *wifiPkey);  // True when connected as a station
