#include "gps_manager.h"

#include <Arduino.h>
#include <time.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#include "../config/pins.h"


/*
 * Manejo del sensor GPS (TinyGPS++) para obtener datos de posición y velocidad.
 * Se inicializa el puerto serial para la comunicación con el módulo GPS y se leen los datos en cada actualización.
 * Los datos se almacenan en una estructura `GPSData` para su uso posterior en el programa.
 */
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

  const uint8_t setRate5Hz[] = {
  0xB5,0x62,
  0x06,0x08,
  0x06,0x00,
  0xC8,0x00,
  0x01,0x00,
  0x01,0x00,
  0xDE,0x6A
 };

gpsSerial.write(setRate5Hz,sizeof(setRate5Hz));

delay(200);

  
}

uint32_t buildUnixTimestamp() {

    if (!gps.date.isValid() ||
        !gps.time.isValid()) {

        return 0;
    }

    struct tm t;

    t.tm_year = gps.date.year() - 1900;
    t.tm_mon  = gps.date.month() - 1;
    t.tm_mday = gps.date.day();

    t.tm_hour = gps.time.hour();
    t.tm_min  = gps.time.minute();
    t.tm_sec  = gps.time.second();

    return mktime(&t);
}


void debugGPSStatus() {
    /*
   * DEBUG GPS STATUS
   */

  static unsigned long lastDebug = 0;

  if (millis() - lastDebug > 2000) {

    Serial.println();
    Serial.println("====== GPS DEBUG ======");

    Serial.print("Chars processed: ");
    Serial.println(gps.charsProcessed());

    Serial.print("Sentences with fix: ");
    Serial.println(gps.sentencesWithFix());

    Serial.print("Location valid: ");
    Serial.println(gps.location.isValid());

    Serial.print("Satellites valid: ");
    Serial.println(gps.satellites.isValid());

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    Serial.println("=======================");

    lastDebug = millis();
  }

  static uint32_t lastGpsUpdate = 0;

   if (gps.location.isUpdated()) {

    Serial.print("GPS update interval: ");

    Serial.println(
        millis() - lastGpsUpdate
    );

    lastGpsUpdate = millis();
}

}

void updateGPS() {

  while (gpsSerial.available() > 0) {

    char c = gpsSerial.read();

    gps.encode(c);
  }

  debugGPSStatus();


  /*
   * UPDATE DATA
   */

  gpsData.valid = gps.location.isValid() &&
                  gps.date.isValid() &&
                  gps.time.isValid() &&
                  gps.satellites.value() >= 4;

  if (gpsData.valid) {

    gpsData.latitude = gps.location.lat();

    gpsData.longitude = gps.location.lng();

    gpsData.speedKmph = gps.speed.kmph();

    gpsData.altitude = gps.altitude.meters();

    gpsData.satellites = gps.satellites.value();

    gpsData.timestamp = buildUnixTimestamp();

    gpsData.timestampMs =
    ((uint64_t)gpsData.timestamp * 1000ULL) +
    (gps.time.centisecond() * 10ULL);

  } else {
    gpsData.latitude = 0;
    gpsData.longitude = 0;
    gpsData.speedKmph = 0;
    gpsData.altitude = 0;
    gpsData.satellites = 0;
    gpsData.timestamp = 0;
    gpsData.timestampMs = 0;
  }
}

GPSData getGPSData() {
  return gpsData;
}

void debugGPSRaw() {

  while (gpsSerial.available()) {

    char c = gpsSerial.read();

    Serial.write(c);
  }
}

