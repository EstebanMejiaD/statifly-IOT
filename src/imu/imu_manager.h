#pragma once

#include "../models/imu_data.h"

bool initIMU();

void updateIMU();

IMUData getIMUData();