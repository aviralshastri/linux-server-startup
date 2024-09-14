#include <LiquidCrystal_I2C.h>

// Set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// Set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  // Initialize LCD
  lcd.init();
  // Turn on LCD backlight                       
  lcd.backlight();
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Check if data is available to read from the serial monitor
  if (Serial.available()) {
    // Read the incoming message
    String message = Serial.readStringUntil('\n');
    
    // Clear the LCD before displaying the new message
    lcd.clear();
    
    // Display the message on the LCD
    displayMessage(message);
  }
  
  delay(100); // Small delay to avoid overwhelming the loop
}

void displayMessage(String message) {
  int messageLength = message.length();
  
  // Display the first line of the message
  lcd.setCursor(0, 0);
  if (messageLength > lcdColumns) {
    lcd.print(message.substring(0, lcdColumns)); // Display up to 16 characters
  } else {
    lcd.print(message);
  }
  
  // Display the second line of the message with ellipsis at the end if necessary
  lcd.setCursor(0, 1);
  if (messageLength > lcdColumns) {
    // Calculate start index for the second line
    int startIndex = messageLength - lcdColumns;
    String secondLine = message.substring(startIndex);

    // Ensure the second line has "..." at the end
    if (messageLength > lcdColumns + 3) {
      secondLine = secondLine.substring(0, lcdColumns - 3) + "...";
    }
    lcd.print(secondLine);
  }
}
