#include <arduinoFFT.h>

// Constants
#define SAMPLES 256              // Must be a power of 2
const double SAMPLING_FREQUENCY = 1000.0; // Sampling frequency in Hz

// Variables
double vReal[SAMPLES]; // Real part
double vImag[SAMPLES]; // Imaginary part (for FFT computation)

// Create FFT object with required parameters
ArduinoFFT FFT = ArduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // ESP32 has a 12-bit ADC
  Serial.println("FFT Example");
}

void loop() {
  /* 1. Collect Samples */
  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = analogRead(A0); // Read analog signal from pin A0
    vImag[i] = 0.0;            // No imaginary part
    delayMicroseconds(1000000 / SAMPLING_FREQUENCY);
  }

  /* 2. Apply FFT */
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD); // Apply windowing
  FFT.compute(FFT_FORWARD);                       // Compute FFT
  FFT.complexToMagnitude();                       // Compute magnitude

  /* 3. Print Results */
  Serial.println("Frequency (Hz) | Amplitude");
  for (int i = 0; i < SAMPLES / 2; i++) { // Only need to display half the bins
    double frequency = (i * SAMPLING_FREQUENCY) / SAMPLES;
    Serial.print(frequency, 1);
    Serial.print(" Hz | ");
    Serial.println(vReal[i], 1);
  }

  delay(1000); // Wait a bit before the next sampling
}