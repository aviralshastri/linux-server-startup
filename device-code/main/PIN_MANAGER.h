#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#include <Arduino.h>

class PIN_MANAGER {
public:
    PIN_MANAGER();  // Constructor to initialize pin modes

    // Process LEDs
    const uint8_t process_led_red = 26;
    const uint8_t process_led_green = 13;
    const uint8_t process_led_blue = 12;

    // Startup LEDs
    const uint8_t startup_led_red = 25;
    const uint8_t startup_led_green = 22;
    const uint8_t startup_led_blue = 16;

    // Buttons
    const uint8_t left_button = 14;
    const uint8_t right_button = 32;
    const uint8_t center_button = 33;
    const uint8_t down_button = 27;
    const uint8_t power_button = 15;
    const uint8_t esc_button = 4;

    // RFID Pins
    const uint8_t sda = 17;
    const uint8_t rst = 5;
    const uint8_t mosi = 23;
    const uint8_t miso = 19;
    const uint8_t sck = 18;

    // Pin setup method to configure pin modes
    void initPins();
};

#endif
