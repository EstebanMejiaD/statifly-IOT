#pragma once

#include "../gps/gps_manager.h"
#include "../imu/imu_manager.h"

bool initSD();

bool startSession();

void appendSessionData(GPSData gps, IMUData imu);

void closeSession();