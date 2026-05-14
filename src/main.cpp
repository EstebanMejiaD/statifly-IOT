#include <Arduino.h>

#include "leds/led_manager.h"

#include "gps/gps_manager.h"

#include "imu/imu_manager.h"

#include "core/state_manager.h"

#include "buttons/button_manager.h"

#include "core/states/off_state.h"
#include "core/states/recording_state.h"
#include "core/states/ble_state.h"


void setup() {

  Serial.begin(115200);

  Serial.println("Statifly V1 Initializing...");

  initLEDs();

  initButton();

  initGPS();

  if (!initIMU()) {

    Serial.println("Failed to find MPU6050 chip!");

    while (1);
  }

  Serial.println("MPU6050 connected successfully!");

  

  updateState(DEVICE_OFF);
}

void loop() {

  updateButton();

  switch (getCurrentState()) {

    case DEVICE_OFF:
      handleOffState();
      break;

    case DEVICE_RECORDING:
      handleRecordingState();
      break;

    case DEVICE_BLE_MODE:
      handleBLEState();
      break;
  }
}