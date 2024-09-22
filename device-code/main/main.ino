#include <Arduino.h>
#include "GENERALS.h"

GENERALS myGenerals;

void setup() {
    Serial.begin(115200);
    delay(3000);

    // Initialize the device
    myGenerals.initialize_device();

    // Add two dummy tags
    Serial.println(myGenerals.add_tag("tag1", "First Tag", "Admin"));
    Serial.println(myGenerals.add_tag("tag2", "Second Tag", "User"));

    // Update tag name
    Serial.println(myGenerals.update_tag_name("tag1", "Updated First Tag"));

    // Update tag role
    Serial.println(myGenerals.update_tag_role("tag2", "Moderator"));

    // List all tags
    String allTags = myGenerals.list_all_tags();
    Serial.println("All Tags:");
    Serial.println(allTags);
}

void loop() {
    // Empty loop
}
