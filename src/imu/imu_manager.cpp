#include "imu_manager.h"

#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define MPU_ADDR 0x68

IMUData imuData;

float gyroOffsetX = 0;
float gyroOffsetY = 0;
float gyroOffsetZ = 0;

bool initIMU() {

    Wire.begin(21, 22);

    Wire.setClock(100000);

    // Wake up MPU
    Wire.beginTransmission(MPU_ADDR);

    Wire.write(0x6B);

    Wire.write(0);

    if (Wire.endTransmission(true) != 0) {
        return false;
    }

    calibrateGyro();

    return true;
}

void calibrateGyro() {

    Serial.println("Calibrating gyro...");

    const int samples = 200;

    long sumX = 0;
    long sumY = 0;
    long sumZ = 0;

    for (int i = 0; i < samples; i++) {

        Wire.beginTransmission(MPU_ADDR);

        Wire.write(0x43);

        if (Wire.endTransmission(false) != 0) {
            continue;
        }

        int bytesReceived =
            Wire.requestFrom(
                (uint8_t)MPU_ADDR,
                (size_t)6,
                true
            );

        if (bytesReceived != 6) {
            continue;
        }

        int16_t rawGyroX =
            (Wire.read() << 8) | Wire.read();

        int16_t rawGyroY =
            (Wire.read() << 8) | Wire.read();

        int16_t rawGyroZ =
            (Wire.read() << 8) | Wire.read();

        sumX += rawGyroX;
        sumY += rawGyroY;
        sumZ += rawGyroZ;

        delay(5);
    }

    gyroOffsetX =
        ((float)sumX / samples) / 131.0f;

    gyroOffsetY =
        ((float)sumY / samples) / 131.0f;

    gyroOffsetZ =
        ((float)sumZ / samples) / 131.0f;

    Serial.println("Gyro calibration complete");

    Serial.print("Offset X: ");
    Serial.println(gyroOffsetX);

    Serial.print("Offset Y: ");
    Serial.println(gyroOffsetY);

    Serial.print("Offset Z: ");
    Serial.println(gyroOffsetZ);
}

void updateIMU() {

    Wire.beginTransmission(MPU_ADDR);

    Wire.write(0x3B);

    if (Wire.endTransmission(false) != 0) {
        return;
    }

    int bytesReceived =
        Wire.requestFrom(
            (uint8_t)MPU_ADDR,
            (size_t)14,
            true
        );

    if (bytesReceived != 14) {
        return;
    }

    int16_t rawAccelX =
        (Wire.read() << 8) | Wire.read();

    int16_t rawAccelY =
        (Wire.read() << 8) | Wire.read();

    int16_t rawAccelZ =
        (Wire.read() << 8) | Wire.read();

    imuData.accelX = rawAccelX / 16384.0f;
    imuData.accelY = rawAccelY / 16384.0f;
    imuData.accelZ = rawAccelZ / 16384.0f;

    // Temperatura
    Wire.read();
    Wire.read();

    int16_t rawGyroX =
        (Wire.read() << 8) | Wire.read();

    int16_t rawGyroY =
        (Wire.read() << 8) | Wire.read();

    int16_t rawGyroZ =
        (Wire.read() << 8) | Wire.read();

    imuData.gyroX = (rawGyroX / 131.0f) - gyroOffsetX;
    imuData.gyroY = (rawGyroY / 131.0f) - gyroOffsetY;
    imuData.gyroZ = (rawGyroZ / 131.0f) - gyroOffsetZ;

    imuData.acceleration =
        sqrt(
            imuData.accelX * imuData.accelX +
            imuData.accelY * imuData.accelY +
            imuData.accelZ * imuData.accelZ
        );



        
}

IMUData getIMUData() {
    return imuData;
}