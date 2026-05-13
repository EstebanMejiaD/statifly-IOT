#include <Arduino.h>
#include "esp32-setup.h"
#include "esp32-functions.h"


void setup() {
  // put your setup code here, to run once:
  
  esp32_serial_setup(115200);
  esp32_io_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (read_button(BUTTON)){
    reset_output(LED1, LED2);
    Serial.println("Button OFF");
  }else{
    set_output(LED1, LED2);
    Serial.println("Button ON");
  }
}

