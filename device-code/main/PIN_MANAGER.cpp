#include "PIN_MANAGER.h"

// Constructor
PIN_MANAGER::PIN_MANAGER() {
}

// Function to initialize pin modes
void PIN_MANAGER::initPins() {
    // Set pin modes for process LEDs
    pinMode(process_led_red, OUTPUT);
    pinMode(process_led_green, OUTPUT);
    pinMode(process_led_blue, OUTPUT);

    // Set pin modes for startup LEDs
    pinMode(startup_led_red, OUTPUT);
    pinMode(startup_led_green, OUTPUT);
    pinMode(startup_led_blue, OUTPUT);

    // Set pin modes for buttons (using internal pull-up resistors)
    pinMode(left_button, INPUT_PULLUP);
    pinMode(right_button, INPUT_PULLUP);
    pinMode(center_button, INPUT_PULLUP);
    pinMode(down_button, INPUT_PULLUP);
    pinMode(power_button, INPUT_PULLUP);
    pinMode(esc_button, INPUT_PULLUP);

    // Set pin modes for RFID pins
    pinMode(sda, OUTPUT);  // Change if required for your RFID module
    pinMode(rst, OUTPUT);
    pinMode(mosi, OUTPUT);
    pinMode(miso, INPUT);
    pinMode(sck, OUTPUT);
}
