#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// Calculate vertical centering with 20px margins
#define TOP_MARGIN 20
#define BOTTOM_MARGIN 20
#define GRAPH_HEIGHT 280 
#define GRAPH_Y (TOP_MARGIN)
#define GRAPH_X 0
#define GRAPH_WIDTH 450 // 480 is the total width 

// Simulated data array
int signalData[] = {5, 15, 20, 18, 10, 25, 30, 15, 10, 20, 10, 25, 18, 12, 8, 15, 20, 18, 12, 8, 5, 15};
int dataSize = sizeof(signalData) / sizeof(signalData[0]);

// Graph data storage
int graphData[GRAPH_WIDTH];
int dataIndex = 0;
int currentColumn = 0;
int lastY = 0;

void setup() {
  // Initialize display
  tft.begin(0x9486); // Replace 0x9486 with your display ID if different
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);
  
  // Initialize graph data array
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    graphData[i] = signalData[0];
  }
  
  // Draw static elements (grid, labels, and legends)
  drawGraphLayout();
  
  // Initial last Y position
  lastY = map(signalData[0], 0, 50, GRAPH_HEIGHT, 0);
}

void loop() {
  // Get next data point
  int currentValue = signalData[dataIndex];
  
  // Map current data point to Y coordinate
  int currentY = map(currentValue, 0, 50, GRAPH_HEIGHT, 0);
  
  // Choose color based on value
  uint16_t color = (currentY <= GRAPH_HEIGHT * 0.75) ? TFT_RED : TFT_YELLOW;
  
  // Shift graph to the right
  for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
    int prevY = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
    int nextY = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);
    
    // Erase previous line
    tft.drawLine(GRAPH_X + x, GRAPH_Y + prevY, GRAPH_X + x - 1, GRAPH_Y + nextY, TFT_BLACK);
    
    // Update graph data
    graphData[x] = graphData[x - 1];
  }
  
  // Add new data point at the beginning (left side)
  graphData[0] = currentValue;
  
  // Redraw graph with new data
  for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
    int y1 = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
    int y2 = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);
    
    uint16_t drawColor = (y1 <= GRAPH_HEIGHT * 0.75) ? TFT_RED : TFT_YELLOW;
    tft.drawLine(GRAPH_X + x, GRAPH_Y + y1, GRAPH_X + x - 1, GRAPH_Y + y2, drawColor);
  }
  
  // Update dynamic info
  displayDynamicInfo(dataIndex);
  
  // Update indices
  dataIndex = (dataIndex + 1) % dataSize;
  
  delay(100); // Fast refresh rate
}

void drawGraphLayout() {
  // Draw graph border
  tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_WHITE);
  
  // Draw static grid lines
  for (int i = 0; i <= 10; i++) {
    int y = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawLine(GRAPH_X, y, GRAPH_X + GRAPH_WIDTH, y, TFT_DARKGREY); // Horizontal grid lines
  }
  for (int i = 0; i <= 10; i++) {
    int x = GRAPH_X + i * (GRAPH_WIDTH / 10);
    tft.drawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY); // Vertical grid lines
  }
  
  // Add legends and static text
  tft.setTextColor(TFT_WHITE);
  
  // dBm scale on the right with larger font and overlapping on graph
  // int dbmLabels[] = {20, 10, 0, -10, -20, -30};
  // for (int i = 0; i < 6; i++) {
  //   int y = GRAPH_Y + i * (GRAPH_HEIGHT / 5) - 5;
  //   tft.setTextSize(2);
  //   tft.setCursor(GRAPH_WIDTH - 70, y);
  //   tft.setTextColor(TFT_LIGHTGREY);
  //   tft.print(dbmLabels[i]);
  //   tft.print(" dBm");
  // }
  
  // Frequency scale at the bottom
  tft.setTextSize(1);
  tft.setCursor(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.setTextColor(TFT_WHITE);
  tft.print("START: 0 Hz");
  tft.setCursor(GRAPH_X + GRAPH_WIDTH - 70, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.print("STOP: 350 MHz");
  
  // Dynamic values section
  tft.setCursor(20, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.print("Freq: ");
  tft.setCursor(20, GRAPH_Y + GRAPH_HEIGHT + 60);
  tft.print("Power: ");
}

void displayDynamicInfo(int index) {
  // Simulate dynamic frequency and dBm updates
  float frequency = 5 + (index % 10) * 0.1; // Simulated frequency in MHz
  float power = -7.0 + (index % 5) * 0.1;    // Simulated power in dBm
  
  // Clear previous values
  tft.fillRect(80, GRAPH_Y + GRAPH_HEIGHT + 40, 150, 40, TFT_BLACK);
  
  // Display new frequency
  tft.setCursor(80, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.print(frequency, 1);
  tft.print(" MHz");
  
  // Display new power
  tft.setCursor(80, GRAPH_Y + GRAPH_HEIGHT + 60);
  tft.setTextColor(TFT_RED);
  tft.print(power, 1);
  tft.print(" dBm");
}





// the one with smooth display of graph moving but blackout refresh...... 
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// Calculate vertical centering with 20px margins
#define TOP_MARGIN 20
#define BOTTOM_MARGIN 20
#define GRAPH_HEIGHT 280 
#define GRAPH_Y (TOP_MARGIN)
#define GRAPH_X 20 // Graph shifted 20px to the right
#define GRAPH_WIDTH 450 // 480 is the total width

// Simulated data array
int signalData[] = {5, 15, 20, 18, 10, 25, 30, 15, 10, 20, 10, 25, 18, 12, 8, 15, 20, 18, 12, 8, 5, 15};
int dataSize = sizeof(signalData) / sizeof(signalData[0]);

// Graph data storage
int graphData[GRAPH_WIDTH];
int dataIndex = 0;

void setup() {
  // Initialize display
  tft.begin(0x9486); // Replace 0x9486 with your display ID if different
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);
  
  // Initialize graph data array
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    graphData[i] = signalData[0];
  }
  
  // Draw static elements (grid, labels, and legends)
  drawGraphLayout();
}

void loop() {
  // Get next data point
  int currentValue = signalData[dataIndex];
  
  // Map current data point to Y coordinate
  int currentY = map(currentValue, 0, 50, GRAPH_HEIGHT, 0);
  
  // Shift graph to the right
  for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
    int prevY = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
    int nextY = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);
    
    // Erase previous line and redraw grid lines
    tft.drawLine(GRAPH_X + x, GRAPH_Y + prevY, GRAPH_X + x - 1, GRAPH_Y + nextY, TFT_BLACK);
    redrawGridLines(x); // Ensure the grid lines stay visible
    
    // Update graph data
    graphData[x] = graphData[x - 1];
  }
  
  // Add new data point at the beginning (left side)
  graphData[0] = currentValue;
  
  // Redraw graph with new data
  for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
    int y1 = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
    int y2 = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);
    
    uint16_t drawColor = (y1 <= GRAPH_HEIGHT * 0.75) ? TFT_RED : TFT_YELLOW;
    tft.drawLine(GRAPH_X + x, GRAPH_Y + y1, GRAPH_X + x - 1, GRAPH_Y + y2, drawColor);
  }
  
  // Update dynamic info
  displayDynamicInfo(dataIndex);
  
  // Update indices
  dataIndex = (dataIndex + 1) % dataSize;
  
  delay(100); // Fast refresh rate
}

void drawGraphLayout() {
  // Draw graph border
  tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_WHITE);
  
  // Draw static grid lines
  for (int i = 0; i <= 10; i++) {
    int y = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawLine(GRAPH_X, y, GRAPH_X + GRAPH_WIDTH, y, TFT_DARKGREY); // Horizontal grid lines
  }
  for (int i = 0; i <= 10; i++) {
    int x = GRAPH_X + i * (GRAPH_WIDTH / 10);
    tft.drawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY); // Vertical grid lines
  }
  
  // Add legends and static text
  tft.setTextColor(TFT_WHITE);
  
  // Frequency scale at the bottom
  tft.setTextSize(1);
  tft.setCursor(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.print("START: 0 Hz");
  tft.setCursor(GRAPH_X + GRAPH_WIDTH - 70, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.print("STOP: 350 MHz");
  
  // Dynamic values section
  tft.setCursor(20, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.print("Freq: ");
  tft.setCursor(20, GRAPH_Y + GRAPH_HEIGHT + 60);
  tft.print("Power: ");
}

void redrawGridLines(int x) {
  // Redraw vertical grid lines
  for (int i = 0; i <= 10; i++) {
    int gridX = GRAPH_X + i * (GRAPH_WIDTH / 10);
    if (gridX == (GRAPH_X + x)) {
      tft.drawLine(gridX, GRAPH_Y, gridX, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY);
    }
  }
  
  // Redraw horizontal grid lines
  for (int i = 0; i <= 10; i++) {
    int gridY = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawPixel(GRAPH_X + x, gridY, TFT_DARKGREY);
  }
}

void displayDynamicInfo(int index) {
  // Simulate dynamic frequency and dBm updates
  float frequency = 5 + (index % 10) * 0.1; // Simulated frequency in MHz
  float power = -7.0 + (index % 5) * 0.1;    // Simulated power in dBm
  
  // Clear previous values
  tft.fillRect(80, GRAPH_Y + GRAPH_HEIGHT + 40, 150, 40, TFT_BLACK);
  
  // Display new frequency
  tft.setCursor(80, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.print(frequency, 1);
  tft.print(" MHz");
  
  // Display new power
  tft.setCursor(80, GRAPH_Y + GRAPH_HEIGHT + 60);
  tft.setTextColor(TFT_RED);
  tft.print(power, 1);
  tft.print(" dBm");
}









// Display which takes input values 
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>4

MCUFRIEND_kbv tft;

// Screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

// Graph area dimensions
#define TOP_MARGIN 20
#define BOTTOM_MARGIN 20
#define GRAPH_HEIGHT 280 
#define GRAPH_Y (TOP_MARGIN)141
#define GRAPH_X 20 // Graph shifted 20px to the right
#define GRAPH_WIDTH 450 // 480 is the total width

// Graph data storage
int graphData[GRAPH_WIDTH];
int dataIndex = 0; // Tracks the current index for input data

void setup() {
  // Initialize display
  tft.begin(0x9486); // Replace 0x9486 with your display ID if different
  tft.setRotation(1); // Landscape orientation
  tft.fillScreen(TFT_BLACK);

  // Initialize graph data array with zeros
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    graphData[i] = 0;
  }

  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("Enter data values (0-50):");

  // Draw static elements (grid, labels, and legends)
  drawGraphLayout();
}

void loop() {
  if (Serial.available() > 0) {
    // Read data from Serial input
    int currentValue = Serial.parseInt();

    // Validate the input range
    if (currentValue < 0 || currentValue > 50) {
      Serial.println("Invalid input! Enter a value between 0 and 50.");
      return;
    }

    // Map current data point to Y coordinate
    int currentY = map(currentValue, 0, 50, GRAPH_HEIGHT, 0);

    // Shift graph to the right
    for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
      int prevY = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
      int nextY = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);

      // Erase previous line and redraw grid lines
      tft.drawLine(GRAPH_X + x, GRAPH_Y + prevY, GRAPH_X + x - 1, GRAPH_Y + nextY, TFT_BLACK);
      redrawGridLines(x); // Ensure the grid lines stay visible

      // Update graph data
      graphData[x] = graphData[x - 1];
    }

    // Add new data point at the beginning (left side)
    graphData[0] = currentValue;

    // Redraw graph with new data
    for (int x = GRAPH_WIDTH - 1; x > 0; x--) {
      int y1 = map(graphData[x], 0, 50, GRAPH_HEIGHT, 0);
      int y2 = map(graphData[x - 1], 0, 50, GRAPH_HEIGHT, 0);

      uint16_t drawColor = (y1 <= GRAPH_HEIGHT * 0.75) ? TFT_RED : TFT_YELLOW;
      tft.drawLine(GRAPH_X + x, GRAPH_Y + y1, GRAPH_X + x - 1, GRAPH_Y + y2, drawColor);
    }

    // Update dynamic info
    displayDynamicInfo(currentValue);

    delay(1); // Small delay to manage input speed
  }
}

void drawGraphLayout() {
  // Draw graph border
  tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, TFT_WHITE);
  
  // Draw static grid lines
  for (int i = 0; i <= 10; i++) {
    int y = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawLine(GRAPH_X, y, GRAPH_X + GRAPH_WIDTH, y, TFT_DARKGREY); // Horizontal grid lines
  }
  for (int i = 0; i <= 10; i++) {
    int x = GRAPH_X + i * (GRAPH_WIDTH / 10);
    tft.drawLine(x, GRAPH_Y, x, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY); // Vertical grid lines
  }
  
  // Add legends and static text
  tft.setTextColor(TFT_WHITE);
  
  // Frequency scale at the bottom
  tft.setTextSize(1);
  tft.setCursor(GRAPH_X, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.print("START: 0 Hz");
  tft.setCursor(GRAPH_X + GRAPH_WIDTH - 70, GRAPH_Y + GRAPH_HEIGHT + 5);
  tft.print("STOP: 350 MHz");
  
  // Dynamic values section
  tft.setCursor(20, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.print("Value: ");
}

void redrawGridLines(int x) {
  // Redraw vertical grid lines
  for (int i = 0; i <= 10; i++) {
    int gridX = GRAPH_X + i * (GRAPH_WIDTH / 10);
    if (gridX == (GRAPH_X + x)) {
      tft.drawLine(gridX, GRAPH_Y, gridX, GRAPH_Y + GRAPH_HEIGHT, TFT_DARKGREY);
    }
  }
  
  // Redraw horizontal grid lines
  for (int i = 0; i <= 10; i++) {
    int gridY = GRAPH_Y + i * (GRAPH_HEIGHT / 10);
    tft.drawPixel(GRAPH_X + x, gridY, TFT_DARKGREY);
  }
}

void displayDynamicInfo(int value) {
  // Clear previous values
  tft.fillRect(80, GRAPH_Y + GRAPH_HEIGHT + 40, 150, 20, TFT_BLACK);
  
  // Display new value
  tft.setCursor(80, GRAPH_Y + GRAPH_HEIGHT + 40);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.print(value);
}


