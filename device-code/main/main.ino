#include "RFID.h"

RFID rfid;  // Create an instance of the RFID class

void setup() {
  Serial.begin(115200);
  rfid.begin();  // Initialize the RFID reader
  
  // Add some authorized tags
  rfid.add_tag("ABCDEF12");
  rfid.add_tag("98765432");
}

void loop() {
  String uid = rfid.scan_tag();
  if (uid != "") {
    Serial.print("UID tag scanned: ");
    Serial.println(uid);
    
    if (rfid.is_authorized(uid)) {
      Serial.println("Access granted!");
    } else {
      Serial.println("Access denied!");
    }
  }
}