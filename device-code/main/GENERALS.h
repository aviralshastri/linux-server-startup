#ifndef GENERALS_H
#define GENERALS_H

#include <Arduino.h>
#include "esp_system.h"
#include <WiFi.h>

class GENERALS {
public:
    static String getUniqueId();
    static bool connectToWiFi(const char* ssid, const char* password);
};

#endif
