#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <arduinoFFT.h>

MCUFRIEND_kbv tft;
ArduinoFFT<double> FFT = ArduinoFFT<double>();

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// FFT parameters
#define SAMPLES 128             // Must be a power of 2
#define SAMPLING_FREQUENCY 1000  // Hz

// Graph area dimensions
#define TOP_MARGIN 20
#define BOTTOM_MARGIN 20
#define GRAPH_HEIGHT 280 
#define GRAPH_Y (TOP_MARGIN)
#define GRAPH_X 20 
#define GRAPH_WIDTH 280 

// Analog input pin
#define SIGNAL_PIN A2

// Data storage
double vReal[SAMPLES];
double vImag[SAMPLES];
int samplesIndex = 0;

// Scrolling variables
int scrollX = 0;
bool scrollInitialized = false;

// Sampling timer
unsigned long previousMicros = 0;
const unsigned long samplingInterval = 1000000 / SAMPLING_FREQUENCY;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  // Initialize display
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);

  // Initialize analog input
  pinMode(SIGNAL_PIN, INPUT);

  // Draw initial graph layout
  drawGraphLayout();

  // Print initialization message
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, SCREEN_HEIGHT - 20);
  tft.print("FFT Signal Analyzer Initialized");
}

void loop() {
  // Precise sampling using micros()
  unsigned long currentMicros = micros();
  
  if (currentMicros - previousMicros >= samplingInterval) {
    previousMicros = currentMicros;

    // Collect samples
    if (samplesIndex < SAMPLES) {
      // Read analog input
      vReal[samplesIndex] = analogRead(SIGNAL_PIN);
      vImag[samplesIndex] = 0;
      samplesIndex++;

      // Scroll the time domain graph in real-time
      scrollTimeDomainGraph(vReal[samplesIndex-1]);

      // Optional: Send data to serial for external monitoring
      Serial.println(vReal[samplesIndex-1]);
    } else {
      // Perform signal analysis
      performFFTAnalysis();
      
      // Reset for next cycle
      samplesIndex = 0;
      scrollX = 0;
      scrollInitialized = false;
    }
  }
}

void scrollTimeDomainGraph(double currentSample) {
  // Initialize scrolling
  if (!scrollInitialized) {
    // Clear time domain area
    tft.fillRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_BLACK);
    drawGraphLayout(); // Redraw grid lines
    scrollInitialized = true;
    scrollX = 0;
  }

  // Map current sample to graph height
  int y = map(currentSample, 0, 1023, GRAPH_HEIGHT, 0);

  // Scroll existing graph left if we've reached graph width
  if (scrollX >= GRAPH_WIDTH) {
    // Manually shift pixels left
    for (int py = 0; py < GRAPH_HEIGHT; py++) {
      for (int px = GRAPH_X; px < GRAPH_X + GRAPH_WIDTH - 1; px++) {
        // Read pixel color from next column and write to current column
        uint16_t pixelColor = tft.readPixel(px + 1, GRAPH_Y + py);
        tft.drawPixel(px, GRAPH_Y + py, pixelColor);
      }
    }
    
    // Redraw grid lines and border to prevent them from being erased
    tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_WHITE);
    drawGridLines();
    
    scrollX = GRAPH_WIDTH - 1;
  }

  // Draw current sample point
  tft.drawPixel(GRAPH_X + scrollX, GRAPH_Y + y, TFT_RED);
  scrollX++;
}

void performFFTAnalysis() {
  // Clear frequency domain area
  tft.fillRect(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 30, GRAPH_WIDTH, 50, TFT_BLACK);
  
  // Windowing function
  FFT.windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  
  // Compute FFT
  FFT.compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  
  // Compute magnitude
  FFT.complexToMagnitude(vReal, vImag, SAMPLES);

  // Find peak frequency and magnitude
  double peakFreq = 0;
  double peakMagnitude = 0;
  
  // Find peak frequency
  for (int i = 1; i < (SAMPLES/2); i++) {
    if (vReal[i] > peakMagnitude) {
      peakMagnitude = vReal[i];
      peakFreq = i * (SAMPLING_FREQUENCY / SAMPLES);
    }
  }

  // Display peak frequency information
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 30);
  tft.print("Peak Freq: ");
  tft.print(peakFreq);
  tft.print(" Hz");
  
  // Display peak magnitude
  tft.setCursor(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.print("Peak Mag: ");
  tft.print(peakMagnitude);
}

void drawGraphLayout() {
  // Draw graph border
  tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_WHITE);
  
  // Draw grid lines
  drawGridLines();
  
  // Title
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(GRAPH_X, GRAPH_Y - 10);
  tft.print("Signal Analysis");
}

void drawGridLines() {
  // Draw grid lines
  for (int i = 0; i <= 10; i++) {
    int y = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawLine(GRAPH_X, y, GRAPH_X + GRAPH_WIDTH, y, TFT_DARKGREY);
    
    int x = GRAPH_X + i * (GRAPH_WIDTH / 10);
    tft.drawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY);
  }
}