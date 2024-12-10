// Arduino code to generate random values and simulate sensor readings

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  randomSeed(analogRead(0)); // Seed the random number generator with noise from an analog pin
}

void loop() {
  // Generate a random number between 0 and 1023 (simulate 10-bit ADC values)
  int randomValue = random(0, 1024);

  // Convert the random value to a voltage-like value (0 to 5V range)
  float voltage = randomValue * (5.0 / 1023.0);

  // Send the voltage over the serial port
  Serial.println(voltage);

  // Wait for 10 milliseconds (100 Hz data generation)
  delay(10);
}
