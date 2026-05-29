#include "imu_manager.h"

#include <Arduino.h>
#include <Wire.h>

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
        Wire.requestFrom((uint8_t)MPU_ADDR,
                         (size_t)6,
                         true);

    if (bytesReceived != 6) {
        return;
    }

    imuData.accelX =
        (Wire.read() << 8) | Wire.read();

    imuData.accelY =
        (Wire.read() << 8) | Wire.read();

    imuData.accelZ =
        (Wire.read() << 8) | Wire.read();
}

IMUData getIMUData() {
    return imuData;
}