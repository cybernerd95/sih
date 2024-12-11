#include <Arduino.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#define BAUD_RATE 115200
#define SAMPLE_RATE 1000
#define CUTOFF_FREQUENCY 150
#define DURATION 10

// Low-pass Butterworth filter implementation
void lowPassFilter(std::vector<float>& data, float cutoff, float fs, int order = 5) {
    float nyquist = 0.5 * fs;
    float normalCutoff = cutoff / nyquist;
    
    // Calculate filter coefficients
    int n = data.size();
    float *b = new float[order+1];
    float *a = new float[order+1];
    
    // Apply Butterworth filter design (simplified for this example)
    // Note: For simplicity, this example does not use a complete filter design algorithm
    // which would require advanced signal processing functions.

    // Simple first-order low-pass filter for illustration:
    float RC = 1.0 / (2 * 3.1416 * cutoff);
    float dt = 1.0 / fs;
    float alpha = dt / (RC + dt);

    for (int i = 1; i < n; i++) {
        data[i] = alpha * data[i] + (1 - alpha) * data[i - 1];
    }

    delete[] b;
    delete[] a;
}

std::vector<float> xData;
std::vector<float> yData;
unsigned long startTime;
bool dataCollectionComplete = false;

void setup() {
    // Start Serial communication
    Serial.begin(BAUD_RATE);
    while (!Serial);  // Wait for serial connection
    std::cout << "Waiting for data collection..." << std::endl; 
}

void loop() {
    if (!dataCollectionComplete) {
        // Collect data for duration
        if (millis() - startTime < DURATION * 1000) {
            if (Serial.available() > 0) {
                String lineData = Serial.readStringUntil('\n');
                lineData.trim();
                
                float sig = lineData.toFloat();
                if (sig != 0.0 || lineData == "0") {
                    xData.push_back(xData.size());
                    yData.push_back(sig);
                }
            }
        } else {
            // Data collection is complete, apply the filter and save the data
            std::cout << "Data collection complete. Applying filter..." << std::endl;
            lowPassFilter(yData, CUTOFF_FREQUENCY, SAMPLE_RATE);
            
            // Save data to file (Arduino doesn't support file I/O like PC)
            // Instead, you might want to send the data over Serial to a PC for saving
            for (size_t i = 0; i < xData.size(); i++) {
                Serial.print(xData[i]);
                Serial.print(",");
                Serial.print(yData[i]);
                Serial.println();
            }

            std::cout << "Filtered data sent via serial." << std::endl;
            dataCollectionComplete = true;
        }
    } else {
        // In this case, the loop is just idle after data collection and saving
        // You can optionally reset or re-trigger the collection by adding conditions here.
    }
}
