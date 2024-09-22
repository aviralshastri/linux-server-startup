#ifndef GENERALS_H
#define GENERALS_H

#include <Arduino.h>
#include "esp_system.h"
#include <WiFi.h>
#include <Preferences.h>

class GENERALS {
public:
    static String getUniqueId();
    static bool connectToWiFi(const char* ssid, const char* password);
    static void write_settings(const String& key, const String& value);
    static String read_settings(const String& key);

private:
    static Preferences preferences;
};

#endif
