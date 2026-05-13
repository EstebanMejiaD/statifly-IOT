#include <Arduino.h>
#include "esp32-setup.h"


// Inicialización del puerto serial para la comunicación con el monitor serie
void esp32_serial_setup(int baudrate) {
  Serial.begin(baudrate);
  Serial.printf("Puerto serie inicializado a %d", baudrate);
  Serial.println();
}


// Fincion para configurar los pines de entrada y salida del ESP32
void esp32_io_setup(void) {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}


