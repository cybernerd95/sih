#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <TinyGPS++.h>
#include <HardwareSerial.h>

TinyGPSPlus gps;
HardwareSerial mySerial(1);

#define RXPin 16 // GPS TX
#define TXPin 17 // GPS RX
#define GPSBaud 9600
#define CSPin 5  // SD card CS pin

void writeFile(fs::FS &fs, const char *path, const char *message) {
    File file = fs.open(path, FILE_WRITE);
    if (!file) return;
    file.print(message);
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
    File file = fs.open(path, FILE_APPEND);
    if (!file) return;
    file.print(message);
    file.close();
}

void setup() {
    Serial.begin(115200);
    mySerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);

    // Initialize SD card
    if (!SD.begin(CSPin)) {
        Serial.println("Card Mount Failed");
        return;
    }

    // Write CSV header
    writeFile(SD, "/gps_data.csv", "Date,Time,Latitude,Longitude,Int1,Int2\n");
}

void loop() {
    while (mySerial.available() > 0) {
        char c = mySerial.read();

        if (gps.encode(c)) {
            if (gps.location.isUpdated() && gps.time.isValid() && gps.date.isValid()) {
                // Get GPS data
                double latitude = gps.location.lat();
                double longitude = gps.location.lng();
                
                // Get GPS time and date
                int year = gps.date.year();
                int month = gps.date.month();
                int day = gps.date.day();
                int hour = gps.time.hour();
                int minute = gps.time.minute();
                int second = gps.time.second();

                // Time zone adjustment (example: UTC+5:30 for IST)
                hour += 5;
                minute += 30;
                if (minute >= 60) {
                    minute -= 60;
                    hour += 1;
                }
                if (hour >= 24) {
                    hour -= 24;
                    day += 1; // Simple day adjustment (not considering month boundaries)
                }

                // Define your integer values here
                int int1 = 42; // Replace with your own logic
                int int2 = 99; // Replace with your own logic

                // Prepare the timestamp
                char timestamp[32];
                snprintf(timestamp, sizeof(timestamp), "%04d-%02d-%02d,%02d:%02d:%02d",
                         year, month, day, hour, minute, second);

                // Prepare the CSV entry
                char entry[128];
                snprintf(entry, sizeof(entry), "%s,%.6f,%.6f,%d,%d\n", 
                         timestamp, latitude, longitude, int1, int2);

                // Append to CSV file
                appendFile(SD, "/gps_data.csv", entry);

                // Debug output
                Serial.printf("Logged: %s", entry);
            } else {
                Serial.println("Waiting for valid GPS data...");
            }
        }
    }
}
