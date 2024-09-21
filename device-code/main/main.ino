#include <WiFi.h>

void setup() {
  Serial.begin(115200); // Initialize Serial communication
  delay(1000); // Wait for a second

  // Get the MAC address
  String macAddress = WiFi.macAddress();

  // Print the MAC address
  Serial.println("Unique MAC Address: " + macAddress);
}

void loop() {
  // Nothing to do here
}
