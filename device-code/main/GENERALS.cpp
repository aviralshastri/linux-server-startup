#include "GENERALS.h"
#include <ArduinoJson.h>

Preferences GENERALS::preferences;

String GENERALS::getUniqueId() {
  uint64_t chipId = ESP.getEfuseMac();
  String uniqueId = String((uint16_t)(chipId >> 32), HEX) + String((uint32_t)chipId, HEX);
  return uniqueId;
}

bool GENERALS::isValidSettingsKey(const String& key) {
  static const char* validKeys[] = {"SP", "WI", "WP", "UI", "UP", "APP", "API", "OEK", "CEK"};
  for (const char* validKey : validKeys) {
    if (key == validKey) {
      return true;
    }
  }
  return false;
}

String GENERALS::readSettingsKey(const String& key) {
  if (!isValidSettingsKey(key)) {
    return "Invalid key";
  }

  preferences.begin("settings-keys", true);
  String value = preferences.getString(key.c_str(), "");
  preferences.end();

  return value;
}

bool GENERALS::writeSettingsKey(const String& key, const String& value) {
  if (!isValidSettingsKey(key)) {
    return false;
  }

  preferences.begin("settings-keys", false);
  bool success = preferences.putString(key.c_str(), value);
  preferences.end();

  return success;
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

void GENERALS::clearNamespace(const char* namespaceName) {
  preferences.begin(namespaceName, false);
  preferences.clear();
  preferences.end();
}

bool GENERALS::initialize_device() {
  clearNamespace("settings-keys");
  clearNamespace("rfid-tags");

  preferences.begin("settings-keys", false);
  preferences.putString("SP", "false");
  preferences.putString("WI", "");
  preferences.putString("WP", "");
  preferences.putString("UI", "admin");
  preferences.putString("UP", "admin");
  preferences.putString("API", "ESP32-SERVER-CONTROLLER");
  preferences.putString("APP", "admin");
  preferences.putString("OEK", "");
  preferences.putString("CEK", "");
  preferences.putString("TA", "[]");
  preferences.end();

  preferences.begin("rfid-tags", false);
  preferences.putInt("TT", 0);
  preferences.end();

  return true;
}

bool isValidRole(const String& role) {
  return (role == "admin" || role == "user");
}

bool isValidName(const String& name) {
  for (char c : name) {
    if (!isalnum(c) && c != ' ') {
      return false;
    }
  }
  return true;
}

String GENERALS::get_list_of_tags() {
  preferences.begin("rfid-tags", true);
  String taJson = preferences.getString("TA", "[]");
  preferences.end();
  return taJson;
}

bool GENERALS::add_tag_to_list(const String& tagToAdd) {
  String taJson = get_list_of_tags();
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, taJson);
  if (error) {
    Serial.println("Failed to parse TA JSON");
    return false;
  }

  JsonArray taArray = doc.as<JsonArray>();
  for (JsonVariant v : taArray) {
    if (v.as<String>() == tagToAdd) {
      return false; 
    }
  }
  taArray.add(tagToAdd);
  String updatedTaJson;
  serializeJson(doc, updatedTaJson);
  preferences.begin("rfid-tags", false);
  bool success = preferences.putString("TA", updatedTaJson);
  preferences.end();

  return success;
}

bool GENERALS::remove_tag_from_list(const String& tagToRemove) {
  String taJson = get_list_of_tags();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, taJson);

  if (error) {
    Serial.println("Failed to parse TA JSON");
    return false;
  }

  JsonArray taArray = doc.as<JsonArray>();
  for (JsonArray::iterator it = taArray.begin(); it != taArray.end(); ++it) {
    if (*it == tagToRemove) {
      taArray.remove(it);
      String updatedTaJson;
      serializeJson(doc, updatedTaJson);
      preferences.begin("rfid-tags", false);
      bool success = preferences.putString("TA", updatedTaJson);
      preferences.end();

      return success;
    }
  }
  return false;
}

String GENERALS::add_tag(const String& id, const String& name, const String& role) {
  if (!isValidRole(role)) {
    return "invalid role: must be 'admin' or 'user'";
  }
  if (!isValidName(name)) {
    return "invalid name: special characters are not allowed";
  }
  if (tag_exists(id)) {
    return "exist";
  }

  preferences.begin("rfid-tags", false);

  int ttValue = preferences.getInt("TT", 0);
  if (ttValue >= 3) {
    preferences.end();
    return "cannot add more than 3 tags";
  }

  DynamicJsonDocument doc(256);
  doc["name"] = name;
  doc["role"] = role;
  String userInfo;
  serializeJson(doc, userInfo);
  bool result = preferences.putString(id.c_str(), userInfo);
  preferences.putInt("TT", ttValue + 1);

  preferences.end();

  if (result) {
    if (add_tag_to_list(id)) {
      return "success";
    } else {
      return "failed to update tag list";
    }
  } else {
    return "failed";
  }
}

String GENERALS::update_tag_name(const String& id, const String& newName) {
  if (!isValidName(newName)) {
    return "invalid name: special characters are not allowed";
  }

  preferences.begin("rfid-tags", false);
  if (!preferences.isKey(id.c_str())) {
    preferences.end();
    return "tag does not exist";
  }

  String currentData = preferences.getString(id.c_str(), "{}");
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, currentData);
  if (error) {
    preferences.end();
    return "failed to parse current data";
  }

  doc["name"] = newName;

  String updatedData;
  serializeJson(doc, updatedData);

  if (preferences.putString(id.c_str(), updatedData)) {
    preferences.end();
    return "name updated successfully";
  } else {
    preferences.end();
    return "failed to save updated data";
  }
}


String GENERALS::update_tag_role(const String& id, const String& newRole) {
  if (!isValidRole(newRole)) {
    return "invalid name: special characters are not allowed";
  }

  preferences.begin("rfid-tags", false);
  if (!preferences.isKey(id.c_str())) {
    preferences.end();
    return "tag does not exist";
  }

  String currentData = preferences.getString(id.c_str(), "{}");
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, currentData);
  if (error) {
    preferences.end();
    return "failed to parse current data";
  }

  doc["role"] = newRole;

  String updatedData;
  serializeJson(doc, updatedData);

  if (preferences.putString(id.c_str(), updatedData)) {
    preferences.end();
    return "role updated successfully";
  } else {
    preferences.end();
    return "failed to save updated data";
  }
}

String GENERALS::remove_tag(const String& id) {
  preferences.begin("rfid-tags", false);

  int ttValue = preferences.getInt("TT", -1);
  if (ttValue <= 1) {
    preferences.end();
    return "at least one tag should exist";
  }

  String existingTag = preferences.getString(id.c_str(), "none");
  if (existingTag == "none") {
    preferences.end();
    return "does not exist";
  }

  preferences.remove(id.c_str());
  preferences.putInt("TT", ttValue - 1);
  preferences.end();

  if (remove_tag_from_list(id)) {
    return "success";
  } else {
    return "failed to update tag list";
  }
}

bool GENERALS::tag_exists(const String& id) {
  preferences.begin("rfid-tags", false);
  String existingTag = preferences.getString(id.c_str(), "none");
  preferences.end();
  return existingTag != "none";
}

String GENERALS::get_tag_details_by_id(const String& id) {
  preferences.begin("rfid-tags", true);
  String tagData = preferences.getString(id.c_str(), "{}");
  preferences.end();
  return tagData;
}

String GENERALS::list_all_tags() {
  String tagsList = "";
  String taJson = get_list_of_tags();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, taJson);

  if (error) {
    return "Failed to parse TA JSON";
  }

  JsonArray taArray = doc.as<JsonArray>();

  for (JsonVariant v : taArray) {
    String id = v.as<String>();
    String tagInfo = get_tag_details_by_id(id);
    tagsList += id + ": " + tagInfo + "\n";
  }

  return tagsList.isEmpty() ? "No tags found." : tagsList;
}