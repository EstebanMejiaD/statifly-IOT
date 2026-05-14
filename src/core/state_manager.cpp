#include "state_manager.h"

#include <Arduino.h>

#include "../leds/led_manager.h"

DeviceState currentState = DEVICE_OFF;

void updateState(DeviceState newState) {

  currentState = newState;

  switch (currentState) {

    case DEVICE_OFF:

      setRGB(0, 0, 0);

      Serial.println("STATE: OFF");

      break;

    case DEVICE_RECORDING:

      setRGB(0, 1, 0);

      Serial.println("STATE: RECORDING");

      break;

    case DEVICE_BLE_MODE:

      setRGB(0, 0, 1);

      Serial.println("STATE: BLE MODE");

      break;
  }
}

DeviceState getCurrentState() {
  return currentState;
}