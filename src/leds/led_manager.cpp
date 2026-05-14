#include <Arduino.h>
#include "led_manager.h"
#include "../config/pins.h"

void initLEDs() {
    pinMode(RGB_RED_PIN, OUTPUT);
    pinMode(RGB_GREEN_PIN, OUTPUT);
    pinMode(RGB_BLUE_PIN, OUTPUT);

    pinMode(BATTERY_LED_PIN, OUTPUT);
}

void setRGB(bool r, bool g, bool b) {
    digitalWrite(RGB_RED_PIN, r);
    digitalWrite(RGB_GREEN_PIN, g);
    digitalWrite(RGB_BLUE_PIN, b);
}

