const int ANALOG_INPUT_PIN = 25; // Analog input pin on ESP32
const int ANALOG_PIN = 12; // Analog output pin on Arduino

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // Set ADC resolution to 12 bits (0–4095)
  analogSetAttenuation(ADC_11db); // Set the ADC input range to 0–3.3V
  pinMode(ANALOG_INPUT_PIN,INPUT);
  pinMode(ANALOG_PIN,OUTPUT);
}

void loop() {
    int randomNumber = random(4095); // Generate a random value between 0 and 3.3
  // int pwmValue = map(randomNumber, 0, 330, 0, 1023); // Map to 8-bit PWM range (0–255)

  analogWrite(ANALOG_PIN, randomNumber); // Output PWM signal
  Serial.print("Sent Voltage: ");
  Serial.println(randomNumber); // Print the value for debugging
  int rawADC = analogRead(ANALOG_INPUT_PIN); // Read raw ADC value
  //float voltage = rawADC * (3.3 / 4095.0); // Convert ADC value to voltage (0–3.3V)

  Serial.print("Received Voltage: ");
  Serial.println(rawADC); // Print voltage with 2 decimal places
  delay(100); // Short delay for stability
}