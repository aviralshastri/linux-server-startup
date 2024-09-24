#include <Arduino.h>
#include "GENERALS.h"

GENERALS myGenerals;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println("Before update:");
  Serial.println(myGenerals.get_tag_details_by_id("tag1"));
  Serial.println("Update result:");
  Serial.println(myGenerals.update_tag_name("tag1", "amay"));
  Serial.println("After update:");
  Serial.println(myGenerals.get_tag_details_by_id("tag1"));
}
void loop() {
}
