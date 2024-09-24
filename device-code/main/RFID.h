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
    
    struct Tag {
        String uid;
        String access_level;
    };

    static const int MAX_TAGS = 10;
    Tag authorized_tags[MAX_TAGS];
    int num_tags;


public:
    RFID(int ss_pin = 17, int rst_pin = 5);
    void begin();
    String scan_tag();
    bool is_authorized(const String& tag, const String& required_level);
    bool add_tag(const String& tag, const String& access_level);
    bool remove_tag(const String& tag);
    bool change_tag(const String& old_tag, const String& new_tag);
    bool change_tag_access_level(const String& tag, const String& new_level);
};

#endif
