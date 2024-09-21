#include "GENERALS.h"

String GENERALS::getUniqueId() {
    uint64_t chipId = ESP.getEfuseMac();
    String uniqueId = String((uint16_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
    return uniqueId;
}

bool GENERALS::connectToWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected.");
        return true;
    } else {
        Serial.println("\nWiFi connection failed.");
        return false;
    }
}
