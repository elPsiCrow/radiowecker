#pragma once

#include <WebServer.h>

extern WebServer server;

void setup_webserver();
void webserver_loop();  // Handle pending HTTP requests
