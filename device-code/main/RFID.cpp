#include "RFID.h"

RFID::RFID(int ss_pin, int rst_pin) : mfrc522(ss_pin, rst_pin), ss_pin(ss_pin), rst_pin(rst_pin), num_tags(0) {}

void RFID::begin() {
    SPI.begin(18, 19, 23, 17); 
    mfrc522.PCD_Init();      
    Serial.println("RFID reader initialized. Ready to scan tags.");
}

String RFID::scan_tag() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return ""; 
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
        return "";
    }

    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    
    delay(1000);
    return content;
}

bool RFID::is_authorized(const String& tag, const String& required_level) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i].uid == tag && authorized_tags[i].access_level == required_level) {
            return true;
        }
    }
    return false;
}

bool RFID::add_tag(const String& tag, const String& access_level) {
    if (num_tags < MAX_TAGS) {
        authorized_tags[num_tags].uid = tag;
        authorized_tags[num_tags].access_level = access_level;
        num_tags++;
        return true;
    }
    return false;
}

bool RFID::remove_tag(const String& tag) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i].uid == tag) {
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
        if (authorized_tags[i].uid == old_tag) {
            authorized_tags[i].uid = new_tag;
            return true;
        }
    }
    return false;
}

bool RFID::change_tag_access_level(const String& tag, const String& new_level) {
    for (int i = 0; i < num_tags; i++) {
        if (authorized_tags[i].uid == tag) {
            authorized_tags[i].access_level = new_level;
            return true;
        }
    }
    return false;
}
