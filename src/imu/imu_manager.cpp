#include "imu_manager.h"

#include <Wire.h>

#include <Adafruit_MPU6050.h>

#include <Adafruit_Sensor.h>

/*
 * Manejo del sensor IMU (MPU6050) para obtener datos de aceleración y giroscopio. 
 * Se inicializa el sensor, se configuran los rangos y se leen los datos en cada actualización. 
 * Los datos se almacenan en una estructura `IMUData` para su uso posterior en el programa.
 */
Adafruit_MPU6050 mpu;
IMUData imuData;

bool initIMU() {

  if (!mpu.begin()) {
    return false;
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  mpu.setGyroRange(MPU6050_RANGE_250_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  return true;
}

void updateIMU() {

  sensors_event_t a, g, temp;

  mpu.getEvent(&a, &g, &temp);

  imuData.accelX = a.acceleration.x;
  imuData.accelY = a.acceleration.y;
  imuData.accelZ = a.acceleration.z;

  imuData.gyroX = g.gyro.x;
  imuData.gyroY = g.gyro.y;
  imuData.gyroZ = g.gyro.z;
}

IMUData getIMUData() {
  return imuData;
}