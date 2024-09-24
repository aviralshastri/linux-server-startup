#ifndef GENERALS_H
#define GENERALS_H

#include <Arduino.h>
#include "esp_system.h"
#include <WiFi.h>
#include <Preferences.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

class GENERALS {
public:
    static String getUniqueId();
    static bool connectToWiFi(const char* ssid, const char* password);
    static void write_settings_keys(const String& key, const String& value);
    static String read_settings_keys(const String& key);
    static bool initialize_device();
    static String add_tag(const String& id, const String& name, const String& role);
    static bool tag_exists(const String& id);
    static String remove_tag(const String& id);
    static String list_all_tags();
    static String update_tag_name(const String& id, const String& newName);
    static String update_tag_role(const String& id, const String& newRole);
    static String get_tag_details_by_id(const String& id);
    static void clearNamespace(const char* namespaceName);

private:
    static Preferences preferences;
    static const int EEPROM_SIZE = 256;
    static int getAddress(const String& varName);
};

#endif
