#include <Wire.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("\nI2C Scanner");
  Wire.begin();
}

void loop() {
  Serial.println("Scanning for I2C devices...");

  for (byte address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found device at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  Serial.println("Scan complete. Waiting 5 seconds before next scan...\n");
  delay(5000);
}
