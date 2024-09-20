#ifndef RFID_H
#define RFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

class RFID {
private:
    MFRC522 mfrc522;
    int ss_pin;
    int rst_pin;
    static const int MAX_TAGS = 10;
    String authorized_tags[MAX_TAGS];
    int num_tags;

    bool is_authorized(const String& tag);

public:
    RFID(int ss_pin = 17, int rst_pin = 5);
    void begin();
    String scan_tag();
    bool add_tag(const String& tag);
    bool remove_tag(const String& tag);
    bool change_tag(const String& old_tag, const String& new_tag);
    bool change_tag_access_level(const String& tag, int new_level);
};

#endif // RFID_H