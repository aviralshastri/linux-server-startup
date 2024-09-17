#include <SPI.h>
#include <MFRC522.h>

// Define the pins used for the MFRC522
#define SS_PIN 17   // SDA pin
#define RST_PIN 5   // RST pin

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);  // Initialize serial communications with the PC
  SPI.begin(18, 19, 23, 17);  // Initialize SPI with custom ESP32 pins
  mfrc522.PCD_Init();    // Initialize MFRC522
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  // If no new card is present, do nothing
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  // If no card could be selected, do nothing
  }

  // Show the UID on the Serial Monitor
  Serial.print("UID tag: ");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  delay(1000);  // Delay to prevent continuous reads
}
