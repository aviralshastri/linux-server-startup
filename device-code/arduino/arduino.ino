#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

int lcdColumns = 16;
int lcdRows = 2;

const int buttonLeftPin = 14;
const int buttonRightPin = 32;

const unsigned long debounceDelay = 50; 
unsigned long lastButtonPress = 0;
bool buttonLeftState = false;
bool buttonRightState = false;

const String messages[] = {"Settings", "SSH", "Network", "Resources", "General"};
const int numMessages = sizeof(messages) / sizeof(messages[0]);
int currentIndex = 0;

class DisplayFunctions {
private:
  LiquidCrystal_I2C* lcd;

public:
  DisplayFunctions(LiquidCrystal_I2C* lcdRef)
    : lcd(lcdRef) {}

  void clearLine(int line) {
    lcd->setCursor(0, line);
    for (int i = 0; i < lcdColumns; i++) {
      lcd->print(" ");
    }
  }

  void displayTextLine(int line, String message) {
    clearLine(line);
    if (message.length() > lcdColumns) {
      message = message.substring(0, lcdColumns - 2) + ".."; 
    }
    lcd->setCursor(0, line);
    lcd->print(message);
  }

  void displayTextCenter(String message) {
    lcd->clear();
    int startPos = (lcdColumns - message.length()) / 2;
    lcd->setCursor(startPos, 0);
    lcd->print(message);
  }

  void displayMessageWithArrows(String firstLineMessage, String secondLineMessage) {
    lcd->clear();
    displayTextCenter(firstLineMessage);
    lcd->setCursor(0, 1);
    lcd->print('<');
    lcd->setCursor(lcdColumns - 1, 1);
    lcd->print('>');

    int availableSpace = lcdColumns - 2;
    if (secondLineMessage.length() <= availableSpace) {
      lcd->setCursor((lcdColumns - secondLineMessage.length()) / 2, 1);
      lcd->print(secondLineMessage);
    } else {
      lcd->setCursor(1, 1);
      lcd->print(secondLineMessage.substring(0, availableSpace - 2) + "..");
    }
  }

  void updateSecondLineTextWithArrow(String newMessage) {
    int availableSpace = lcdColumns - 2;
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
    lcd->setCursor(lcdColumns - 1, 1);
    lcd->print('>');
  }
};

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
DisplayFunctions df(&lcd);

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(buttonLeftPin, INPUT_PULLUP);
  pinMode(buttonRightPin, INPUT_PULLUP);

  df.displayMessageWithArrows("Selected", messages[currentIndex]);
}

void loop() {
  unsigned long currentTime = millis();
  bool currentLeftState = digitalRead(buttonLeftPin) == LOW;
  bool currentRightState = digitalRead(buttonRightPin) == LOW;
  if (currentLeftState && !buttonLeftState && (currentTime - lastButtonPress > debounceDelay)) {
    lastButtonPress = currentTime;
    currentIndex = (currentIndex - 1 + numMessages) % numMessages;
    df.displayMessageWithArrows("Selected", messages[currentIndex]);
  }

  if (currentRightState && !buttonRightState && (currentTime - lastButtonPress > debounceDelay)) {
    lastButtonPress = currentTime;
    currentIndex = (currentIndex + 1) % numMessages;
    df.displayMessageWithArrows("Selected", messages[currentIndex]);
  }

  buttonLeftState = currentLeftState;
  buttonRightState = currentRightState;

  delay(10);
}
