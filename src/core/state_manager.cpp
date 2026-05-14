#include "state_manager.h"

#include <Arduino.h>

#include "../leds/led_manager.h"

#include "../storage/sd_manager.h"

DeviceState currentState = DEVICE_OFF;

void updateState(DeviceState newState) {

  if (currentState == newState) {
    return;
  }

  // ===== EXIT CURRENT STATE =====
  switch (currentState) {

    case DEVICE_RECORDING:

      closeSession();

      break;

    default:
      break;
  }

  // ===== CHANGE STATE =====
  currentState = newState;

  // ===== ENTER NEW STATE =====
  switch (currentState) {

    case DEVICE_OFF:

      setRGB(0, 0, 0);

      Serial.println("STATE: OFF");

      break;

    case DEVICE_RECORDING:

      setRGB(0, 1, 0);

      Serial.println("STATE: RECORDING");

      startSession();

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