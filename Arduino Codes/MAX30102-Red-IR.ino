#include <Wire.h>
#include "MAX30105.h"

MAX30105 max30102;

void setup() {
	Serial.begin(9600);
	if (max30102.begin() == false) {
		Serial.println("MAX30102 was not found. Please check wiring/power.");
		while (1);
	}
	max30102.setup();
}

void loop() {
	Serial.print(" R[");
	Serial.print(max30102.getRed());
	Serial.print("] IR[");
	Serial.print(max30102.getIR());
	Serial.println("]");
}