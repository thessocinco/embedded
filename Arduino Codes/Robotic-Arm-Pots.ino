#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int pot1 = A0;
int pot2 = A1;
int pot3 = A2;
int pot4 = A3;

int valPot1;
int valPot2;
int valPot3;
int valPot4;

void setup()
{
  servo1.attach(11);
  servo2.attach(10);
  servo3.attach(9);
  servo4.attach(6);
}

void loop()
{

  valPot1 = analogRead(pot1);
  valPot1 = map (valPot1, 0, 1023, 0, 180);   
  servo1.write(valPot1);                      
  delay(5);
  
  valPot2 = analogRead(pot2);
  valPot2 = map (valPot2, 0, 1023, 0, 180);
  servo2.write(valPot2);
  delay(5);

  valPot3 = analogRead(pot3);
  valPot3 = map (valPot3, 0, 1023, 0, 180);
  servo3.write(valPot3);
  delay(5);

  valPot4 = analogRead(pot4);
  valPot4 = map (valPot4, 0, 1023, 0, 180);
  servo4.write(valPot4);
  delay(5);
}