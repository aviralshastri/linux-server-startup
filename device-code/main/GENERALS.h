#ifndef GENERALS_H
#define GENERALS_H

#include <Arduino.h>
#include "esp_system.h"
#include <WiFi.h>
#include <Preferences.h>
#include <ArduinoJson.h>

class GENERALS
{
public:
  static String readSettingsKey(const String &key);
  static bool writeSettingsKey(const String &key, const String &value);
  static String getUniqueId();
  static bool connectToWiFi(const char *ssid, const char *password);
  static bool initialize_device();
  static String add_tag(const String &id, const String &name, const String &role);
  static bool tag_exists(const String &id);
  static String remove_tag(const String &id);
  static String list_all_tags();
  static String update_tag_name(const String &id, const String &newName);
  static String update_tag_role(const String &id, const String &newRole);
  static String get_tag_details_by_id(const String &id);
  static void clearNamespace(const char *namespaceName);
  static String get_list_of_tags();
  static bool add_tag_to_list(const String &tagToAdd);
  static bool remove_tag_from_list(const String &tagToRemove);

private:
  static Preferences preferences;
  static bool isValidSettingsKey(const String &key);
};

#endif
