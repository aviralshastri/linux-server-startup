#include <EEPROM.h>  // Include EEPROM library

// Custom data to store in EEPROM
String data1 = "WP:421dfsa2fds234fgdas";
String data2 = "WI:dfas342dasgfsgettret";
String data3 = "UI:fdas342gfd453tgresgf";
String data4 = "UP:faw45fgvds64gfdstre";

// EEPROM memory addresses for storing strings
int eepromStartAddr = 50; // Start storing from address 50 to avoid lower memory usage

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Initialize EEPROM with size (512 bytes is the default for ESP32)
  EEPROM.begin(512);

  // Store data into EEPROM
  storeDataInEEPROM(data1, eepromStartAddr);
  storeDataInEEPROM(data2, eepromStartAddr + data1.length() + 1); // Add 1 for null terminator
  storeDataInEEPROM(data3, eepromStartAddr + data1.length() + data2.length() + 2);
  storeDataInEEPROM(data4, eepromStartAddr + data1.length() + data2.length() + data3.length() + 3);

  // Read data back from EEPROM
  Serial.println("Reading data back from EEPROM:");
  Serial.println(readDataFromEEPROM(eepromStartAddr, data1.length()));
  Serial.println(readDataFromEEPROM(eepromStartAddr + data1.length() + 1, data2.length()));
  Serial.println(readDataFromEEPROM(eepromStartAddr + data1.length() + data2.length() + 2, data3.length()));
  Serial.println(readDataFromEEPROM(eepromStartAddr + data1.length() + data2.length() + data3.length() + 3, data4.length()));

  // Make sure to commit the changes to EEPROM
  EEPROM.commit();
}

void loop() {
  // The loop is left empty as we are only storing and reading data
}

// Function to store a string in EEPROM
void storeDataInEEPROM(String data, int startAddr) {
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(startAddr + i, data[i]); // Write each character of the string
  }
  EEPROM.write(startAddr + data.length(), '\0'); // Add null terminator
  EEPROM.commit();  // Commit the data to EEPROM
}

// Function to read a string from EEPROM
String readDataFromEEPROM(int startAddr, int length) {
  char data[length + 1];  // Create a buffer to hold the string (including null terminator)
  for (int i = 0; i < length; i++) {
    data[i] = EEPROM.read(startAddr + i);
  }
  data[length] = '\0';  // Add null terminator to the buffer
  return String(data);   // Convert the char array back to a String
}
