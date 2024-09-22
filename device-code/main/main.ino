#include <EEPROM.h>
#define WP_ADDR 0
#define WI_ADDR 32
#define UI_ADDR 64
#define UP_ADDR 96

#define EEPROM_SIZE 128
void setup() {
  Serial.begin(115200);
  delay(2000);
  EEPROM.begin(EEPROM_SIZE);

  writeString(WP_ADDR, "WP:421dfsa2fds234fgdas");
  writeString(WI_ADDR, "WI:dfas342dasgfsgettret");
  writeString(UI_ADDR, "UI:fdas342gfd453tgresgf");
  writeString(UP_ADDR, "UP:faw45fgvds64gfdstre");

  EEPROM.commit();
  Serial.println("Stored strings:");
  Serial.println(readString(WP_ADDR));
  Serial.println(readString(WI_ADDR));
  Serial.println(readString(UI_ADDR));
  Serial.println(readString(UP_ADDR));
}

void loop() {
  // Nothing to do in the loop
}

void writeString(int addr, String data) {
  int len = data.length();
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + i, data[i]);
  }
  EEPROM.write(addr + len, '\0'); // Null terminator
}

String readString(int addr) {
  String data = "";
  char c;
  do {
    c = EEPROM.read(addr++);
    if (c != '\0') data += c;
  } while (c != '\0' && addr < EEPROM_SIZE);
  return data;
}