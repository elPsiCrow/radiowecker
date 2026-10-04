#pragma once

void httpOtaRunDeferredBoot(void);  // Boot-time half of the web-triggered GitHub release update
void handleStartHttpUpdate(void);   // POST /cmd/startHttpUpdate
