#include "sd_manager.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 5

File sessionFile;

bool firstEntry = true;

String currentFilename = "";

bool initSD() {

    if (!SD.begin(SD_CS_PIN)) {

        Serial.println("SD initialization failed!");

        return false;
    }

    Serial.println(SD.cardType());
    Serial.println("SD initialized!");

    return true;
}

bool startSession() {

    int sessionNumber = 1;

    while (true) {

        currentFilename = "/session_" + String(sessionNumber) + ".json";

        if (!SD.exists(currentFilename)) {
            break;
        }

        sessionNumber++;
    }

    sessionFile = SD.open(currentFilename, FILE_WRITE);

    if (!sessionFile) {

        Serial.println("Failed to create session file!");

        return false;
    }

    sessionFile.println("[");

    firstEntry = true;

    Serial.println("Session started: " + currentFilename);

    return true;
}

void appendSessionData(GPSData gps, IMUData imu) {

    if (!sessionFile) {
        return;
    }

    if (!firstEntry) {
        sessionFile.println(",");
    }

    String json = "{";

    json += "\"latitude\":" + String(gps.latitude, 6) + ",";
    json += "\"longitude\":" + String(gps.longitude, 6) + ",";

    json += "\"accelX\":" + String(imu.accelX, 2) + ",";
    json += "\"accelY\":" + String(imu.accelY, 2) + ",";
    json += "\"accelZ\":" + String(imu.accelZ, 2);

    json += "}";

    sessionFile.print(json);

    sessionFile.flush();

    firstEntry = false;
}

void closeSession() {

    if (!sessionFile) {
        return;
    }

    sessionFile.println();
    sessionFile.println("]");

    sessionFile.flush();

    sessionFile.close();

    Serial.println("Session closed");
}