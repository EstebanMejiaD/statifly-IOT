#include <Arduino.h>

#include "esp32-setup.h"
#include "leds/led_manager.h"

#include "gps/gps_manager.h"

#include "imu/imu_manager.h"

#include "core/state_manager.h"

#include "buttons/button_manager.h"

#include "core/states/off_state.h"
#include "core/states/recording_state.h"
#include "core/states/ble_state.h"


void setup() {

  esp32_serial_setup(115200);

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