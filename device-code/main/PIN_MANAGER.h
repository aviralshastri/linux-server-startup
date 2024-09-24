#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#include <Arduino.h>

class PIN_MANAGER {
public:
    PIN_MANAGER();

    const uint8_t process_led_red = 26;
    const uint8_t process_led_green = 13;
    const uint8_t process_led_blue = 12;

    const uint8_t startup_led_red = 25;
    const uint8_t startup_led_green = 22;
    const uint8_t startup_led_blue = 16;

    const uint8_t left_button = 14;
    const uint8_t right_button = 32;
    const uint8_t center_button = 33;
    const uint8_t down_button = 27;
    const uint8_t reset_button = 15;
    const uint8_t esc_button = 4;


    void initPins();
};

#endif
