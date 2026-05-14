#include "gps_manager.h"

#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include "../config/pins.h"

TinyGPSPlus gps;

HardwareSerial gpsSerial(1);

GPSData gpsData;

void initGPS() {

  gpsSerial.begin(
    9600,
    SERIAL_8N1,
    GPS_RX_PIN,
    GPS_TX_PIN
  );
}

void updateGPS() {

  while (gpsSerial.available() > 0) {

    char c = gpsSerial.read();

    gps.encode(c);
  }

  gpsData.valid = gps.location.isValid();

  if (gpsData.valid) {

    gpsData.latitude = gps.location.lat();

    gpsData.longitude = gps.location.lng();

    gpsData.speedKmph = gps.speed.kmph();

    gpsData.altitude = gps.altitude.meters();

    gpsData.satellites = gps.satellites.value();
  }
}

GPSData getGPSData() {
  return gpsData;
}