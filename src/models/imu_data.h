#pragma once
#include <stdint.h>
struct IMUData {

    // RAW
    int16_t accelRawX;
    int16_t accelRawY;
    int16_t accelRawZ;

    int16_t gyroRawX;
    int16_t gyroRawY;
    int16_t gyroRawZ;

    // Convertidos
    float accelX;
    float accelY;
    float accelZ;

    float gyroX;
    float gyroY;
    float gyroZ;

    float acceleration;
};