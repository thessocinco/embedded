#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int trigPin = 9; 
const int oledResetPin = -1; 

Adafruit_SSD1306 display(128, 64, &Wire, oledResetPin);

void setup() {
  pinMode(trigPin, OUTPUT); 
  Serial.begin(9600);  

  if (!display.begin(SSD1306_I2C_ADDRESS, 0x3D)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  Serial.println("Ultrasonic pulse sent");

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Pulse sent");
  display.display();

  delay(1000);
}
