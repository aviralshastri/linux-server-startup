#include "RFID.h"

RFID rfid;

void setup() {
  Serial.begin(115200);
  rfid.begin();
  
  rfid.add_tag("230B4EFB", "admin");
  rfid.add_tag("8B36D84F", "user");
}

void loop() {
  String uid = rfid.scan_tag();
  if (uid != "") {
    Serial.print("UID tag scanned: ");
    Serial.println(uid);
    
    if (rfid.is_authorized(uid, "admin")) {
      Serial.println("Access granted for admin!");
    } else if (rfid.is_authorized(uid, "user")) {
      Serial.println("Access granted for user!");
    } else {
      Serial.println("Access denied!");
    }
  }
}
