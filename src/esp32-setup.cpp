#include <Arduino.h>
#include "esp32-setup.h"


// Inicialización del puerto serial para la comunicación con el monitor serie
void esp32_serial_setup(int baudrate) {
  Serial.begin(baudrate);
  Serial.printf("Statifly inicializado a %d", baudrate);
  Serial.println();
}




