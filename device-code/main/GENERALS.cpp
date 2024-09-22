#include "GENERALS.h"

Preferences GENERALS::preferences;

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

void GENERALS::write_settings(const String& key, const String& value) {
    preferences.begin("my-app", false);
    if (key == "setup" || key == "encryption_key" || key == "onboard_key") {
        preferences.putString(key.c_str(), value);
        Serial.println("Data written: " + key + " = " + value);
    } else {
        Serial.println("Invalid key: " + key);
    }
    preferences.end();
}

String GENERALS::read_settings(const String& key) {
    preferences.begin("my-app", false); // Start Preferences
    String value = preferences.getString(key.c_str(), "default");
    preferences.end();
    return value;
}
