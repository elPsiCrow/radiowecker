#include "net/ota.h"

#include <ArduinoOTA.h>

#include "app/app_config.h"
#include "app/app_state.h"
#include "ui/ui_manager.h"

static void ota_onStart();
static void ota_onEnd();
static void ota_onProgress(unsigned int progress, unsigned int total);
static void ota_onError(ota_error_t error);

static const char kOtaTitle[] = "Updating Sketch";

// Last percentage shown, so the display is only updated when it changes.
static uint32_t oldprc;

// Funktion zur Vorbereitung der OTA-Aktualisierung
void setup_ota() {
  // Setze den Hostnamen und das Passwort für OTA-Updates
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Setzt den Hostnamen für OTA
  ArduinoOTA.setPassword(OTA_PASSWORD);  // Setzt das Passwort für OTA

  // Registriere Callback-Routinen für OTA-Ereignisse
  ArduinoOTA.onStart(ota_onStart);        // Callback, wenn die OTA-Aktualisierung beginnt
  ArduinoOTA.onEnd(ota_onEnd);            // Callback, wenn die OTA-Aktualisierung endet
  ArduinoOTA.onProgress(ota_onProgress);  // Callback zur Anzeige des Fortschritts
  ArduinoOTA.onError(ota_onError);        // Callback bei einem Fehler während der OTA-Aktualisierung

  // Starte die OTA-Verarbeitung
  ArduinoOTA.begin();  // Beginnt die OTA-Aktualisierung
}

// Callback-Funktion, die aufgerufen wird, wenn die OTA-Aktualisierung beginnt
static void ota_onStart() {
  oldprc = 0;
  ui.showProgress(kOtaTitle, 0);
}

static void ota_onEnd() {
  ui.showMessage(kOtaTitle, StatusTone::Ok, "Done");
}

static void ota_onProgress(unsigned int progress, unsigned int total) {
  uint32_t prc = progress / (total / 100);
  if (prc > oldprc) {
    ui.showProgress(kOtaTitle, (uint8_t)prc);
    oldprc = prc;
  }
}

// Callback-Funktion, die bei einem Fehler während der OTA-Aktualisierung aufgerufen wird
static void ota_onError(ota_error_t error) {
  char err[80];
  // Formatierte Fehlermeldung
  sprintf(err, "Error[%u]: ", error);
  // Bestimme die spezifische Fehlermeldung basierend auf dem Fehlercode
  if (error == OTA_AUTH_ERROR) sprintf(err, "Error[%u]: Auth Failed", error);
  else if (error == OTA_BEGIN_ERROR) sprintf(err, "Error[%u] Begin Failed", error);
  else if (error == OTA_CONNECT_ERROR) sprintf(err, "Error[%u] Connect Failed", error);
  else if (error == OTA_RECEIVE_ERROR) sprintf(err, "Error[%u] Receive Failed", error);
  else if (error == OTA_END_ERROR) sprintf(err, "Error[%u] End Failed", error);

  ui.showMessage("OTA Fehler", StatusTone::Error, err);
}