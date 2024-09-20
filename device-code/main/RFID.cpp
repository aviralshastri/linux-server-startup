#include "RFID.h"

RFID::RFID(int ss_pin, int rst_pin) : mfrc522(ss_pin, rst_pin), ss_pin(ss_pin), rst_pin(rst_pin), num_tags(0) {}

void RFID::begin() {
    SPI.begin(18, 19, 23, 17); 
    mfrc522.PCD_Init();      
    Serial.println("RFID reader initialized. Ready to scan tags.");
}

String RFID::scan_tag() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return "";  // If no new card is present, return empty string
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return "";  // If no card could be selected, return empty string
    }

    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    
    delay(1000);  // Delay to prevent continuous reads
    return content;
}

bool RFID::is_authorized(const String& tag) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i] == tag) {
            return true;
        }
    }
    return false;
}

bool RFID::add_tag(const String& tag) {
    if (num_tags < MAX_TAGS && !is_authorized(tag)) {
        authorized_tags[num_tags++] = tag;
        return true;
    }
    return false;
}

bool RFID::remove_tag(const String& tag) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i] == tag) {
            for (int j = i; j < num_tags - 1; j++) {
                authorized_tags[j] = authorized_tags[j + 1];
            }
            num_tags--;
            return true;
        }
    }
    return false;
}

bool RFID::change_tag(const String& old_tag, const String& new_tag) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i] == old_tag) {
            authorized_tags[i] = new_tag;
            return true;
        }
    }
    return false;
}

bool RFID::change_tag_access_level(const String& tag, int new_level) {
    return is_authorized(tag);
}