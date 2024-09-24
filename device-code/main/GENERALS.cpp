#include "GENERALS.h"
#include <ArduinoJson.h>
#include <vector>

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
  preferences.putString("WP", "");
  preferences.putString("WI", "");
  preferences.putString("UI", "");
  preferences.putString("UP", "");
  preferences.putString("CEK", "");
  preferences.putString("OEK", "");
  preferences.end();

  preferences.begin("rfid-tags", false);
  preferences.putInt("TT", 0);
  preferences.end();

  return true;
}

void GENERALS::write_settings_keys(const String& key, const String& value) {
  preferences.begin("settings-keys", false);
  if (key == "SP" || key == "EK" || key == "OEK" || key == "TT") {
    preferences.putString(key.c_str(), value);
    Serial.println("Data written: " + key + " = " + value);
  } else {
    Serial.println("Invalid key: " + key);
  }
  preferences.end();
}

String GENERALS::read_settings_keys(const String& key) {
  preferences.begin("settings-keys", false);
  String value = preferences.getString(key.c_str(), "none");
  preferences.end();
  return value;
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

String GENERALS::add_tag(const String& id, const String& name, const String& role) {
    if (!isValidRole(role)) {
        return "invalid role: must be 'Admin' or 'User'";
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
        return "success";
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
    return "success";
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
  preferences.begin("rfid-tags", false);
  int ttValue = preferences.getInt("TT", 0);

  for (int i = 0; i < ttValue; i++) {
    String tagKey = "tag" + String(i + 1);
    String tagInfo = preferences.getString(tagKey.c_str(), "none");
    if (tagInfo != "none") {
      tagsList += tagKey + ": " + tagInfo + "\n";
    }
  }

  preferences.end();
  return tagsList.isEmpty() ? "No tags found." : tagsList;
}
