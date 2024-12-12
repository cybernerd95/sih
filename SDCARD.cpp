#include "FS.h"
#include "SD.h"
#include "SPI.h"

int counter = 0; // Number counter

void createDir(fs::FS &fs, const char * path) {
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)) {
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void readFile(fs::FS &fs, const char * path) {
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void setup() {
    Serial.begin(115200);
    if(!SD.begin()) {
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    // Write CSV header
    writeFile(SD, "/data.csv", "Number,Seconds,Minutes,Hours\n");
}

void loop() {
    // Increment the counter
    counter++;

    // Calculate time in seconds, minutes, and hours
    unsigned long milliseconds = millis();
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    seconds %= 60; // Remaining seconds
    minutes %= 60; // Remaining minutes

    // Prepare the CSV entry
    char entry[64];
    snprintf(entry, sizeof(entry), "%d,%lu,%lu,%lu\n", counter, seconds, minutes, hours);

    // Append to CSV file
    appendFile(SD, "/data.csv", entry);

    // Debug output
    Serial.printf("Logged: %s", entry);

    // Delay for 10 seconds
    delay(10000);
}
