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






































































































Date,Time,Latitude,Longitude,Int1,Int2
2024-12-13,04:05:39,18.493151,74.019941,42,99
2024-12-13,04:05:39,18.493151,74.019941,42,99
2024-12-13,04:05:40,18.493154,74.019935,42,99
2024-12-13,04:05:40,18.493154,74.019935,42,99
2024-12-13,04:05:41,18.493158,74.019932,42,99
2024-12-13,04:05:41,18.493158,74.019932,42,99
2024-12-13,04:05:42,18.493153,74.019925,42,99
2024-12-13,04:05:42,18.493153,74.019925,42,99
2024-12-13,04:05:43,18.493185,74.019907,42,99
2024-12-13,04:05:43,18.493185,74.019907,42,99
2024-12-13,04:05:44,18.493192,74.019904,42,99
2024-12-13,04:05:44,18.493192,74.019904,42,99
2024-12-13,04:05:45,18.493223,74.019887,42,99
2024-12-13,04:05:45,18.493223,74.019887,42,99
2024-12-13,04:05:46,18.493248,74.019875,42,99
2024-12-13,04:05:46,18.493248,74.019875,42,99
2024-12-13,04:05:47,18.493267,74.019863,42,99
2024-12-13,04:05:47,18.493267,74.019863,42,99
2024-12-13,04:05:48,18.493294,74.019852,42,99
2024-12-13,04:05:48,18.493294,74.019852,42,99
2024-12-13,04:05:49,18.493335,74.019836,42,99
2024-12-13,04:05:49,18.493335,74.019836,42,99
2024-12-13,04:05:50,18.493449,74.019783,42,99
2024-12-13,04:05:50,18.493449,74.019783,42,99
2024-12-13,04:05:51,18.493509,74.019754,42,99
2024-12-13,04:05:51,18.493509,74.019754,42,99
2024-12-13,04:05:52,18.493503,74.019755,42,99
2024-12-13,04:05:52,18.493503,74.019755,42,99
2024-12-13,04:05:53,18.493543,74.019735,42,99
2024-12-13,04:05:53,18.493543,74.019735,42,99
2024-12-13,04:05:54,18.493573,74.019717,42,99
2024-12-13,04:05:54,18.493573,74.019717,42,99
2024-12-13,04:05:55,18.493615,74.019695,42,99
2024-12-13,04:05:55,18.493615,74.019695,42,99
2024-12-13,04:05:56,18.493614,74.019697,42,99
2024-12-13,04:05:56,18.493614,74.019697,42,99
2024-12-13,04:05:57,18.493606,74.019695,42,99
2024-12-13,04:05:57,18.493606,74.019695,42,99
2024-12-13,04:05:58,18.493606,74.019690,42,99
2024-12-13,04:05:58,18.493606,74.019690,42,99
2024-12-13,04:05:59,18.493600,74.019683,42,99
2024-12-13,04:05:59,18.493600,74.019683,42,99
2024-12-13,04:06:00,18.493597,74.019671,42,99
2024-12-13,04:06:00,18.493597,74.019671,42,99
2024-12-13,04:06:01,18.493629,74.019652,42,99
2024-12-13,04:06:01,18.493629,74.019652,42,99
2024-12-13,04:06:02,18.493635,74.019636,42,99
2024-12-13,04:06:02,18.493635,74.019636,42,99
2024-12-13,04:06:03,18.493640,74.019635,42,99
2024-12-13,04:06:03,18.493640,74.019635,42,99
2024-12-13,04:06:04,18.493636,74.019631,42,99
2024-12-13,04:06:04,18.493636,74.019631,42,99
2024-12-13,04:06:05,18.493635,74.019623,42,99
2024-12-13,04:06:05,18.493635,74.019623,42,99
2024-12-13,04:06:06,18.493634,74.019624,42,99
2024-12-13,04:06:06,18.493634,74.019624,42,99
2024-12-13,04:06:07,18.493637,74.019613,42,99
2024-12-13,04:06:07,18.493637,74.019613,42,99
2024-12-13,04:06:08,18.493624,74.019615,42,99
2024-12-13,04:06:08,18.493624,74.019615,42,99
2024-12-13,04:06:09,18.493615,74.019612,42,99
2024-12-13,04:06:09,18.493615,74.019612,42,99
2024-12-13,04:06:10,18.493629,74.019601,42,99
2024-12-13,04:06:10,18.493629,74.019601,42,99
2024-12-13,04:06:11,18.493634,74.019597,42,99
2024-12-13,04:06:11,18.493634,74.019597,42,99
2024-12-13,04:06:12,18.493628,74.019600,42,99
2024-12-13,04:06:12,18.493628,74.019600,42,99
2024-12-13,04:06:13,18.493636,74.019595,42,99
2024-12-13,04:06:13,18.493636,74.019595,42,99
2024-12-13,04:06:19,18.493647,74.019598,42,99
2024-12-13,04:06:19,18.493647,74.019598,42,99
2024-12-13,04:06:20,18.493639,74.019606,42,99
2024-12-13,04:06:20,18.493639,74.019606,42,99
2024-12-13,04:06:21,18.493802,74.019536,42,99
2024-12-13,04:06:21,18.493802,74.019536,42,99
2024-12-13,04:06:22,18.493885,74.019501,42,99
2024-12-13,04:06:22,18.493885,74.019501,42,99
2024-12-13,04:06:23,18.493929,74.019476,42,99
2024-12-13,04:06:23,18.493929,74.019476,42,99
2024-12-13,04:06:24,18.493910,74.019485,42,99
2024-12-13,04:06:24,18.493910,74.019485,42,99
2024-12-13,04:06:25,18.493894,74.019499,42,99
2024-12-13,04:06:25,18.493894,74.019499,42,99
2024-12-13,04:06:26,18.493903,74.019497,42,99
2024-12-13,04:06:26,18.493903,74.019497,42,99
2024-12-13,04:06:27,18.493909,74.019493,42,99
2024-12-13,04:06:27,18.493909,74.019493,42,99
2024-12-13,04:06:28,18.493930,74.019484,42,99
2024-12-13,04:06:28,18.493930,74.019484,42,99
2024-12-13,04:06:29,18.493957,74.019470,42,99
2024-12-13,04:06:29,18.493957,74.019470,42,99
