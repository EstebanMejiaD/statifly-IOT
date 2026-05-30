#include <Arduino.h>

#include "../../gps/gps_manager.h"
#include "../../imu/imu_manager.h"
#include "../../storage/sd_manager.h"

unsigned long lastSensorRead = 0;

void handleRecordingState() {
    
    updateGPS();

    if (millis() - lastSensorRead < 100) {
        return;
    }

    lastSensorRead = millis();

    updateIMU();

    GPSData gps = getGPSData();

    // No guardar datos si el GPS aún no tiene FIX válido
    if (!gps.valid ||
        gps.latitude == 0 ||
        gps.longitude == 0) {

        static unsigned long lastMessage = 0;

        if (millis() - lastMessage > 2000) {

            Serial.print("Waiting GPS FIX... Satellites: ");
            Serial.println(gps.satellites);

            lastMessage = millis();
        }

        return;
    }

    IMUData imu = getIMUData();

    Serial.print("Latitude: ");
    Serial.print(gps.latitude, 6);

    Serial.print(", Longitude: ");
    Serial.println(gps.longitude, 6);

    appendSessionData(gps, imu);
}