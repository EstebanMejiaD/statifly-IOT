#include <Arduino.h>

#include "../../gps/gps_manager.h"
#include "../../imu/imu_manager.h"

unsigned long lastSensorRead = 0;

void handleRecordingState() {

    updateGPS();

    updateIMU();

    if (millis() - lastSensorRead >= 100) {

        lastSensorRead = millis();

        GPSData gps = getGPSData();

        IMUData imu = getIMUData();

        if (gps.valid) {

            Serial.print("Latitude: ");
            Serial.print(gps.latitude, 6);

            Serial.print(", Longitude: ");
            Serial.println(gps.longitude, 6);
        }

        Serial.print("Accel X: ");
        Serial.println(imu.accelX);
    }
}