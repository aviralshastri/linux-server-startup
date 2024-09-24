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
    // Retrieves the unique ID of the ESP device
    static String getUniqueId();

    // Connects the device to a WiFi network using SSID and password
    static bool connectToWiFi(const char* ssid, const char* password);

    // Writes a value to a specific settings key
    static void write_settings_keys(const String& key, const String& value);

    // Reads a value from a specific settings key
    static String read_settings_keys(const String& key);

    // Initializes the device by clearing and setting default values for namespaces
    static bool initialize_device();

    // Adds an RFID tag with a specific ID, name, and role
    static String add_tag(const String& id, const String& name, const String& role);

    // Checks if a tag with the specified ID exists
    static bool tag_exists(const String& id);

    // Removes a tag by ID
    static String remove_tag(const String& id);

    // Lists all tags in the system
    static String list_all_tags();

    // Updates the name of a tag by its ID
    static String update_tag_name(const String& id, const String& newName);

    // Updates the role of a tag by its ID
    static String update_tag_role(const String& id, const String& newRole);

    // Retrieves the tag details for a specific ID
    static String get_tag_details_by_id(const String& id);

    // Clears all data within a specified namespace
    static void clearNamespace(const char* namespaceName);

private:
    // Instance of Preferences for managing key-value storage
    static Preferences preferences;

    // EEPROM size for storing small data
    static const int EEPROM_SIZE = 256;

    // Helper function to get the address for a variable in EEPROM
    static int getAddress(const String& varName);
};

#endif
