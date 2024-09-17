// Define button GPIO pins
const int buttonPins[] = {32, 33, 27, 14, 4, 15};  // Button pins
const int numButtons = 6;  // Number of buttons
int buttonStates[numButtons];  // Current button states
int lastButtonStates[numButtons];  // Previous button states

const int outputPin = 35;  // GPIO pin to toggle on button press
bool outputState = LOW;   // Initial state of GPIO pin 2

// Setup function
void setup() {
  Serial.begin(115200);
  
  // Initialize button pins as INPUT_PULLUP
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonStates[i] = HIGH; 
    lastButtonStates[i] = HIGH;
  }
  
  // Initialize GPIO 2 as OUTPUT
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, outputState);  // Start with pin 2 LOW
}

// Loop function
void loop() {
  for (int i = 0; i < numButtons; i++) {
    buttonStates[i] = digitalRead(buttonPins[i]);

    // Check for button press (transition from HIGH to LOW)
    if (buttonStates[i] == LOW && lastButtonStates[i] == HIGH) {
      Serial.println("Button pressed");
      
      // Toggle the output pin state
      outputState = !outputState;
      digitalWrite(outputPin, outputState);  // Set GPIO 2 to the new state

      delay(200);  // Debounce delay to prevent multiple toggles
    }

    lastButtonStates[i] = buttonStates[i];  // Update last button state
  }
}
