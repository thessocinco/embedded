#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int echoPin = 10; 
const int oledResetPin = -1;
Adafruit_SSD1306 display(128, 64, &Wire, oledResetPin);

long duration; 
int distance; 

void setup() {
  pinMode(echoPin, INPUT); 
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
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;

  Serial.print("Distance cm: ");
  Serial.println(distance);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");
  display.display();

  delay(1000);
}
