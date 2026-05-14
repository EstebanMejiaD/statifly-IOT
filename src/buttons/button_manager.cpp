#include <Arduino.h>
#include "../config/pins.h"
#include "../core/state_manager.h"


bool lastButtonState = HIGH;

unsigned long buttonPressStart = 0;

void initButton() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void updateButton() {

    bool currentButton = digitalRead(BUTTON_PIN);

    // pressed
    if (currentButton == LOW && lastButtonState == HIGH) {
        buttonPressStart = millis();
    }

    // released
    if (currentButton == HIGH && lastButtonState == LOW) {

        unsigned long pressDuration = millis() - buttonPressStart;

        if (pressDuration > 1500) {

            DeviceState current = getCurrentState();

            if (current == DEVICE_OFF) {
                updateState(DEVICE_RECORDING);
            }
            else {
                updateState(DEVICE_OFF);
            }
        }
    }

    lastButtonState = currentButton;
}