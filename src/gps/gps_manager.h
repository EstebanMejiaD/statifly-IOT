#pragma once

#include "../models/gps_data.h"
#include <stdint.h>
void initGPS();

void updateGPS();

GPSData getGPSData();

void debugGPSRaw();

void debugGPSStatus();

uint32_t buildUnixTimestamp();