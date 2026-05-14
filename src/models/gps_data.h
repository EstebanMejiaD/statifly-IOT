#pragma once

struct GPSData
{
    double latitude = 0.0;
    double longitude = 0.0;

    int satellites = 0;

    double speedKmph = 0.0;

    double altitude = 0.0;

    bool valid = false;
};