#include "imu_manager.h"

#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define MPU_ADDR 0x68

IMUData imuData;

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

    return true;
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

    imuData.gyroX = rawGyroX / 131.0f;
    imuData.gyroY = rawGyroY / 131.0f;
    imuData.gyroZ = rawGyroZ / 131.0f;

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