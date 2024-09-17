#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

// Initialize the RTC
RTC_DS3231 rtc;

// Initialize the LCD with the I2C address, columns, and rows
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change the address if necessary (0x27 is common)

// Define the I2C SDA and SCL pins
#define SDA_PIN 21
#define SCL_PIN 22

// Button Pins
const int buttonLeftPin = 14;
const int buttonRightPin = 32;
const int menuSwitchPin = 33;

// Constants for debouncing
const unsigned long debounceDelay = 50; 
unsigned long lastButtonPress = 0;
bool buttonLeftState = false;
bool buttonRightState = false;
bool menuButtonState = false;

// Menu mode timeout
const unsigned long menuTimeout = 30000;  // 30 seconds
unsigned long lastActivityTime = 0;

// Menu messages
const String messages[] = {"Settings", "SSH", "Network", "Resources", "General"};
const int numMessages = sizeof(messages) / sizeof(messages[0]);
int currentIndex = 0;

// Mode Flags
bool inMenuMode = false;

class DisplayFunctions {
private:
  LiquidCrystal_I2C* lcd;

public:
  DisplayFunctions(LiquidCrystal_I2C* lcdRef)
    : lcd(lcdRef) {}

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

  void updateSecondLineTextWithArrow(String newMessage) {
    int availableSpace = 14;
    clearLine(1);

    if (newMessage.length() <= availableSpace) {
      lcd->setCursor((availableSpace - newMessage.length()) / 2 + 1, 1);
      lcd->print(newMessage);
    } else {
      lcd->setCursor(1, 1);
      lcd->print(newMessage.substring(0, availableSpace - 2) + "..");
    }

    // Redraw `<` and `>`
    lcd->setCursor(0, 1);
    lcd->print('<');
    lcd->setCursor(15, 1);
    lcd->print('>');
  }
};

DisplayFunctions df(&lcd);

void setup() {
  Serial.begin(9600);

  // Initialize I2C communication and RTC
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power");
  }

  lcd.init();  // Initialize the LCD
  lcd.backlight();

  // Button setup
  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);
  pinMode(menuSwitchPin, INPUT_PULLUP);

  lastActivityTime = millis();
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

void loop() {
  unsigned long currentTime = millis();

  // Handle menu switching
  bool menuButtonPressed = digitalRead(menuSwitchPin) == LOW;
  if (menuButtonPressed && !menuButtonState && (currentTime - lastButtonPress > debounceDelay)) {
    inMenuMode = !inMenuMode;
    lastActivityTime = currentTime;  // Reset the inactivity timer
    if (inMenuMode) {
      df.displayMessageWithArrows("Menu", messages[currentIndex]);
    }
  }
  menuButtonState = menuButtonPressed;

  // If in menu mode, handle button navigation
  if (inMenuMode) {
    bool currentLeftState = digitalRead(buttonLeftPin) == LOW;
    bool currentRightState = digitalRead(buttonRightPin) == LOW;

    if (currentLeftState && !buttonLeftState && (currentTime - lastButtonPress > debounceDelay)) {
      lastButtonPress = currentTime;
      currentIndex = (currentIndex - 1 + numMessages) % numMessages;
      df.displayMessageWithArrows("Menu", messages[currentIndex]);
      lastActivityTime = currentTime;  // Reset the inactivity timer
    }

    if (currentRightState && !buttonRightState && (currentTime - lastButtonPress > debounceDelay)) {
      lastButtonPress = currentTime;
      currentIndex = (currentIndex + 1) % numMessages;
      df.displayMessageWithArrows("Menu", messages[currentIndex]);
      lastActivityTime = currentTime;  // Reset the inactivity timer
    }

    buttonLeftState = currentLeftState;
    buttonRightState = currentRightState;

    // Check for inactivity timeout (30 seconds)
    if (currentTime - lastActivityTime > menuTimeout) {
      inMenuMode = false;
      lastActivityTime = currentTime;  // Reset for clock display
    }
  } else {
    displayClock();  // If not in menu mode, display the clock
  }

  delay(200);
}

// Function to get the day name
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

// Function to get the month name
String monthName(int month) {
  switch (month) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "March";
    case 4: return "April";
    case 5: return "May";
    case 6: return "June";
    case 7: return "July";
    case 8: return "August";
    case 9: return "Sept";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "Err";
  }
}
