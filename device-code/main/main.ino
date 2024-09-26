#include <WiFi.h>
#include "SETUP.h"

const char* apSSID = "ESP32_Access_Point";
const char* apPassword = "123456789";
const char* validUsername = "admin";
const char* validPassword = "admin";

SETUP setupManager(apSSID, apPassword, validUsername, validPassword);

void setup() {
    Serial.begin(115200);
    setupManager.begin();
}

void loop() {
    setupManager.handleClient();
}
