#include <Wire.h>
#include "MAX30105.h"

MAX30105 max30102;

long samplesTaken = 0;
long unblockedValue;
long startTime;

void setup() {
  Serial.begin(9600);
  if (max30102.begin(Wire, I2C_SPEED_FAST) == false) {
    Serial.println("MAX30102 was not found. Please check wiring/power. ");
    while (1);
  }

  byte ledBrightness = 0xFF;
  byte sampleAverage = 4;
  byte ledMode = 2;
  int sampleRate = 400;
  int pulseWidth = 411;
  int adcRange = 2048;
  
  max30102.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  max30102.setPulseAmplitudeRed(0);
  max30102.setPulseAmplitudeGreen(0);

  unblockedValue = 0;
  for (byte x = 0 ; x < 32 ; x++) {
    unblockedValue += max30102.getIR();
  }
  unblockedValue /= 32;
  startTime = millis();
}

void loop() {
  samplesTaken++;
  Serial.print("IR[");
  Serial.print(max30102.getIR());
  Serial.print("] Hz[");
  Serial.print((float)samplesTaken / ((millis() - startTime) / 1000.0), 2);
  Serial.print("]");

  long currentDelta = max30102.getIR() - unblockedValue;

  Serial.print(" delta[");
  Serial.print(currentDelta);
  Serial.print("]");

  if (currentDelta > (long)100) {
    Serial.print(" Something is there!");
  }
  Serial.println();
}