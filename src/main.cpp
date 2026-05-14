#include <Arduino.h>

#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// ==================== PINS ====================
#include "./config/pins.h"
// ==================== LEDS ====================
#include "./leds/led_manager.h"
// ==================== DEVICE STATE ====================
#include "models/device_state.h"




// Create GPS and Serial objects
TinyGPSPlus gps;
HardwareSerial gpsSerial(1); // Use UART1 for GPS

// Create MPU6050 object
Adafruit_MPU6050 mpu;

unsigned long lastSensorRead = 0;



DeviceState currentState = DEVICE_OFF;

// ==================== BUTTON ====================
bool lastButtonState = HIGH;
unsigned long buttonPressStart = 0;
bool buttonPressed = false;

// ==================== FUNCTION DECLARATIONS ====================
void displayData();

void setup() {

  // ==================== GPIO ====================
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize LEDs
  initLEDs();

  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("Statifly V1 Initializing...");

  // Initialize GPS UART
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17


  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip!");
    while (1);
  }
  Serial.println("MPU6050 connected successfully!");

  // Configure MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}



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

void handleButton() {

  bool currentButton = digitalRead(BUTTON_PIN);

  // Button pressed
  if (currentButton == LOW && lastButtonState == HIGH) {
    buttonPressStart = millis();
    buttonPressed = true;
  }

  // Button released
  if (currentButton == HIGH && lastButtonState == LOW) {

    unsigned long pressDuration = millis() - buttonPressStart;

    // LONG PRESS
    if (pressDuration > 1500) {

      if (currentState == DEVICE_OFF) {
        updateState(DEVICE_RECORDING);
      }
      else if (currentState == DEVICE_RECORDING) {
        updateState(DEVICE_OFF);
      }
      else if (currentState == DEVICE_BLE_MODE) {
        updateState(DEVICE_OFF);
      }

    }

    buttonPressed = false;
  }

  lastButtonState = currentButton;
}

void loop() {

  // ==================== GPS ====================
  while (gpsSerial.available() > 0) {

    char c = gpsSerial.read();

    // DEBUG opcional:
    // Serial.write(c);

    gps.encode(c);
  }

  // ==================== BUTTON ====================
  handleButton();

  // ==================== RECORDING ====================
  if (currentState == DEVICE_RECORDING) {

    if (millis() - lastSensorRead >= 100) {

      lastSensorRead = millis();

      displayData();
    }
  }

  delay(10);
}

void displayData() {
  // GPS Data
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Longitude: ");
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println("Waiting GPS signal...");
  }



  // MPU6050 Data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print Accelerometer data
  Serial.print("Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" m/s^2, Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(" m/s^2, Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  // Print Gyroscope data
  Serial.print("Gyro X: ");
  Serial.print(g.gyro.x);
  Serial.print(" rad/s, Y: ");
  Serial.print(g.gyro.y);
  Serial.print(" rad/s, Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  // Optional: Add GPS timestamp
  if (gps.time.isUpdated()) {
    Serial.print("Time: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
  }
}
