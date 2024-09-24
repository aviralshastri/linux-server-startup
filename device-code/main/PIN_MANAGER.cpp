#include "PIN_MANAGER.h"

PIN_MANAGER::PIN_MANAGER() {
}

void PIN_MANAGER::initPins() {
    pinMode(process_led_red, OUTPUT);
    pinMode(process_led_green, OUTPUT);
    pinMode(process_led_blue, OUTPUT);

    pinMode(startup_led_red, OUTPUT);
    pinMode(startup_led_green, OUTPUT);
    pinMode(startup_led_blue, OUTPUT);

    pinMode(left_button, INPUT_PULLUP);
    pinMode(right_button, INPUT_PULLUP);
    pinMode(center_button, INPUT_PULLUP);
    pinMode(down_button, INPUT_PULLUP);
    pinMode(reset_button, INPUT_PULLUP);
    pinMode(esc_button, INPUT_PULLUP);

}
