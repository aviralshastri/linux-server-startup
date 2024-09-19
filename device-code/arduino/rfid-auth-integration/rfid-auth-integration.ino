#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

RTC_DS3231 rtc;

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 17 
#define RST_PIN 5 
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define SDA_PIN 21
#define SCL_PIN 22

const int buttonLeftPin = 14;
const int buttonRightPin = 32;
const int rfidButtonPin = 15;

const unsigned long debounceDelay = 50;
unsigned long lastButtonPress = 0;
bool buttonLeftState = false;
bool buttonRightState = false;

const unsigned long menuTimeout = 30000; 
unsigned long lastActivityTime = 0;

const String messages[] = {"Settings", "SSH", "Network", "Resources", "General"};
const int numMessages = sizeof(messages) / sizeof(messages[0]);
int currentIndex = 0;

bool inMenuMode = false;
bool authenticated = false;

class DisplayFunctions {
private:
  LiquidCrystal_I2C* lcd;

public:
  DisplayFunctions(LiquidCrystal_I2C* lcdRef) : lcd(lcdRef) {}

  void clearLine(int line) {
    lcd->setCursor(0, line);
    for (int i = 0; i < 16; i++) {
      lcd->print(" ");
    }
  }

  void displayTextLine(int line, String message) {
    clearLine(line);
    if (message.length() > 16) {
      message = message.substring(0, 14) + ".."; 
    }
    lcd->setCursor(0, line);
    lcd->print(message);
  }

  void displayTextCenter(String message) {
    lcd->clear();
    int startPos = (16 - message.length()) / 2;
    lcd->setCursor(startPos, 0);
    lcd->print(message);
  }

  void displayMessageWithArrows(String firstLineMessage, String secondLineMessage) {
    lcd->clear();
    displayTextCenter(firstLineMessage);
    lcd->setCursor(0, 1);
    lcd->print('<');
    lcd->setCursor(15, 1);
    lcd->print('>');

    int availableSpace = 14;
    if (secondLineMessage.length() <= availableSpace) {
      lcd->setCursor((16 - secondLineMessage.length()) / 2, 1);
      lcd->print(secondLineMessage);
    } else {
      lcd->setCursor(1, 1);
      lcd->print(secondLineMessage.substring(0, availableSpace - 2) + "..");
    }
  }
};

DisplayFunctions df(&lcd);

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power");
  }

  lcd.init();
  lcd.backlight();

  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(rfidButtonPin, INPUT_PULLUP);

  SPI.begin(18, 19, 23, 17);
  mfrc522.PCD_Init(); 

  lastActivityTime = millis();
  df.displayTextCenter("Clock Mode");
}

void displayClock() {
  DateTime now = rtc.now();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dayName(now.dayOfTheWeek()));
  lcd.print(", ");
  lcd.print(monthName(now.month()));
  lcd.print(" ");
  lcd.print(now.day());
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print((now.hour() < 10 ? "0" : "") + String(now.hour()));
  lcd.print(':');
  lcd.print((now.minute() < 10 ? "0" : "") + String(now.minute()));
  lcd.print(':');
  lcd.print((now.second() < 10 ? "0" : "") + String(now.second()));
}

bool checkRFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.print("UID: ");
  Serial.println(content);
  
  return true;
}

void loop() {
  unsigned long currentTime = millis();

  if (checkRFID()) {
    authenticated = true;
    inMenuMode = true;
    lastActivityTime = currentTime;
    currentIndex = 0; 
    df.displayMessageWithArrows("Menu", messages[currentIndex]);
    Serial.println("Entered Menu Mode");
    delay(1000);
  }

  if (authenticated && inMenuMode) {
    bool currentLeftState = digitalRead(buttonLeftPin) == LOW;
    bool currentRightState = digitalRead(buttonRightPin) == LOW;

    if (currentLeftState && !buttonLeftState && (currentTime - lastButtonPress > debounceDelay)) {
      lastButtonPress = currentTime;
      currentIndex = (currentIndex - 1 + numMessages) % numMessages;
      df.displayMessageWithArrows("Menu", messages[currentIndex]);
      lastActivityTime = currentTime;
    }

    if (currentRightState && !buttonRightState && (currentTime - lastButtonPress > debounceDelay)) {
      lastButtonPress = currentTime;
      currentIndex = (currentIndex + 1) % numMessages;
      df.displayMessageWithArrows("Menu", messages[currentIndex]);
      lastActivityTime = currentTime;
    }

    buttonLeftState = currentLeftState;
    buttonRightState = currentRightState;

    if (currentTime - lastActivityTime > menuTimeout) {
      inMenuMode = false;
      authenticated = false;
      df.displayTextCenter("Clock Mode");
    }
  } else {
    displayClock();
  }

  delay(200);
}


String dayName(int dayOfWeek) {
  switch (dayOfWeek) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    default: return "Err";
  }
}


String monthName(int month) {
  switch (month) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "March";
    case 4: return "April";
    case 5: return "May";
    case 6: return "June";
    case 7: return "July";
    case 8: return "Aug";
    case 9: return "Sept";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "Err";
  }
}