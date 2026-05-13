#include <Arduino.h>
#include "esp32-functions.h"

bool read_button(int button_pin) {
 return digitalRead(button_pin);
}

void set_output(int out1, int out2) {
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
}

void reset_output(int out1, int out2) {
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);
}