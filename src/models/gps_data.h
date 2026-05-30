#pragma once
#include <stdint.h>
struct GPSData
{
    double latitude = 0.0;
    double longitude = 0.0;

    int satellites = 0;

    double speedKmph = 0.0;

    double altitude = 0.0;

    uint32_t timestamp = 0;

    uint64_t timestampMs = 0;

    bool valid = false;
};