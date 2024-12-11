#include <Wire.h>
#include <SPI.h>
#include<WiFi.h>
#include<WiFiClient.h>
WiFiClient client;
void setup() {
  WiFi.begin();
  Wire.begin();
  Serial.begin(115200);
  Wire.beginTransmission(0x48);
  Wire.write(0x01);
  Wire.write(0x73);
  Wire.write(0xE3);
  Wire.endTransmission();
  SPI.begin(); 
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  Serial.println(WiFi.localIP());
  }
void loop()
{
  Wire.beginTransmission(0x48);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.requestFrom(0x48, 2);
  int16_t sig = Wire.read() << 8 | Wire.read();
  //double v=sig*2.048/32768;
  Serial.println(sig);
  
  int16_t voltageData = (sig);
  byte highByte = (voltageData >> 8) & 0xFF;
  byte lowByte = voltageData & 0xFF;
  digitalWrite(5, LOW);
  SPI.transfer(highByte);
  SPI.transfer(lowByte);
  digitalWrite(5, HIGH);
  client.println(sig);
  delay(1);
}
