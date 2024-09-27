#include <Wire.h>
#include "MAX30105.h"
MAX30105 max30102;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  if (max30102.begin(Wire, I2C_SPEED_FAST) == false) {
    Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }
  max30102.setup(0);
  max30102.enableDIETEMPRDY();
}

void loop() {
  float temp = max30102.readTemperature();

  Serial.print("temperatureC=");
  Serial.print(temp, 4);

  float tempF = max30102.readTemperatureF();

  Serial.print(" temperatureF=");
  Serial.print(tempF, 4);

  Serial.println();
}