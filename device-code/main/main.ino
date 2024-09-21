const int buttonPin = 15; // Pin where the button is connected
const unsigned long continuousPressTime = 3000; // Time (in milliseconds) for continuous press
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

unsigned long pressStartTime = 0;
int pressCount = 0;
bool isPressed = false;
unsigned long lastPressTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
  Serial.begin(115200);
}

void loop() {
  // Read the button state
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed
  if (buttonState == LOW) {
    if (!isPressed) {
      // Button just pressed
      isPressed = true;
      pressStartTime = millis(); // Record the time when button was pressed
    }
    
    // Check if the continuous press time exceeds the threshold
    if (isPressed && (millis() - pressStartTime >= continuousPressTime)) {
      // Continuous press detected
      Serial.println("Continuous press detected!");
      pressCount = 0; // Reset press count for triple press detection
      delay(debounceDelay); // Wait for debounce
      while (digitalRead(buttonPin) == LOW); // Wait for button release
      isPressed = false; // Reset button state
      delay(500); // Delay to avoid immediate detection of triple press
    }
  } else {
    // Button is released
    if (isPressed) {
      // Check the timing for triple press detection
      unsigned long currentTime = millis();
      if (currentTime - lastPressTime <= 500) { // Time between presses for triple press
        pressCount++;
        if (pressCount == 3) {
          // Triple press detected
          Serial.println("Triple press detected!");
          pressCount = 0; // Reset for next pattern
        }
      } else {
        // Reset the press count if too much time has passed
        pressCount = 1; // Start counting this press
      }
      lastPressTime = currentTime; // Update the last press time
      isPressed = false; // Reset button state
    }
  }
}
