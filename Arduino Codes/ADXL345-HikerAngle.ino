#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#define OLED_ADDR   0x3C
#define SSD1306_LCDHEIGHT 64

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif;

float AdvntureX, AdvntureY, AdvntureZ;
float AngleRoll, AnglePitch;
uint32_t LoopTimer;
float KalmanAngleRoll = 0, KalmanUncertaintyAngleRoll = 2 * 2;
float KalmanAnglePitch = 0, KalmanUncertaintyAnglePitch = 2 * 2;
float KalmanADVNTUREOutput[] = {0, 0};

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void kalman_ADVNTURE(float KalmanState, float KalmanUncertainty, 
float KalmanInput, float KalmanMeasurement) {
  KalmanState = KalmanState + 0.004 * KalmanInput;
  KalmanUncertainty = KalmanUncertainty + 0.004 * 0.004 * 4 * 4;
  float KalmanGain = KalmanUncertainty * 1 / (1 * KalmanUncertainty + 3 * 3);
  KalmanState = KalmanState + KalmanGain * (KalmanMeasurement - KalmanState);
  KalmanUncertainty = (1 - KalmanGain) * KalmanUncertainty;
  KalmanADVNTUREOutput[0] = KalmanState;
  KalmanADVNTUREOutput[1] = KalmanUncertainty;
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); 
  Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); 
  Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      AdvntureX = AdvntureX * 31.2;
      AdvntureY = AdvntureY * 31.2;
      AdvntureZ = AdvntureZ * 31.2;
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      AdvntureX = AdvntureX * 15.6;
      AdvntureY = AdvntureY * 15.6;
      AdvntureZ = AdvntureZ * 15.6;
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      AdvntureX = AdvntureX * 7.8;
      AdvntureY = AdvntureY * 7.8;
      AdvntureZ = AdvntureZ * 7.8;
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      AdvntureX = AdvntureX * 3.9;
      AdvntureY = AdvntureY * 3.9;
      AdvntureZ = AdvntureZ * 3.9;
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

void setup(void) {
  Serial.begin(9600);
  Serial.println("Accelerometer Calibration By: ADVNTURE "); Serial.println("");

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  display.drawPixel(0, 0, WHITE);
  display.drawPixel(128, 0, WHITE);
  display.drawPixel(0, 64, WHITE);
  display.drawPixel(128, 64, WHITE);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,30);

  Wire.setClock(400000);
  Wire.begin();
  delay(500);
  
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }

   accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);

  displaySensorDetails();
  
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop(void) {

  sensors_event_t advntureEvent;
  accel.getEvent(&advntureEvent);
  
  AdvntureX = advntureEvent.acceleration.x / 9.81;
  AdvntureY = advntureEvent.acceleration.y / 9.81;
  AdvntureZ = advntureEvent.acceleration.z / 9.81;

  AngleRoll = atan(AdvntureY / sqrt(AdvntureX * AdvntureX 
  + AdvntureZ * AdvntureZ)) * 1 / (3.141592653589793238462643383279502884197 / 180);
  AnglePitch = -atan(AdvntureX / sqrt(AdvntureY * AdvntureY 
  + AdvntureZ * AdvntureZ)) * 1 / (3.141592653589793238462643383279502884197 / 180);

  kalman_ADVNTURE(KalmanAngleRoll, KalmanUncertaintyAngleRoll, 0, AngleRoll);
  KalmanAngleRoll = KalmanADVNTUREOutput[0];
  KalmanUncertaintyAngleRoll = KalmanADVNTUREOutput[1];
  kalman_ADVNTURE(KalmanAnglePitch, KalmanUncertaintyAnglePitch, 0, AnglePitch);
  KalmanAnglePitch = KalmanADVNTUREOutput[0];
  KalmanUncertaintyAnglePitch = KalmanADVNTUREOutput[1];

  display.clearDisplay(); 

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  display.print(" Roll Angle: ");
  display.print(KalmanAngleRoll);

  Serial.print(" Roll Angle: ");
  Serial.print(KalmanAngleRoll);
  Serial.print("°");

  display.print("\n Pitch Angle: ");
  display.print(KalmanAnglePitch);
  display.print("\n");

  Serial.print("\n Pitch Angle: ");
  Serial.print(KalmanAnglePitch);
  Serial.print("°\n");

  display.display(); 

bool severeSlopeDisplayed = false;

if (KalmanAnglePitch > 15 || KalmanAnglePitch < -15 || 
    KalmanAngleRoll > 15 || KalmanAngleRoll < -15) {

    if (!severeSlopeDisplayed) {
        display.clearDisplay(); 
        display.setTextSize(2); 
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        display.println(" Severe \n Slope...");

        Serial.println(" Warning: Severe Slope...");

        display.display();
        delay(200);

        severeSlopeDisplayed = true;
    }
} else {
    severeSlopeDisplayed = false;

    if ((KalmanAnglePitch > 10 || KalmanAnglePitch < -10 || 
         KalmanAngleRoll > 10 || KalmanAngleRoll < -10)
         && !(KalmanAnglePitch > 15 || KalmanAnglePitch < -15 || 
              KalmanAngleRoll > 15 || KalmanAngleRoll < -15)) {

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        display.println(" Steep \n Angle...");

        Serial.println(" Warning: Steep Angle...");

        display.display();
        delay(200);
    }
}

if (KalmanAnglePitch <= 10 && KalmanAnglePitch >= -10 && 
    KalmanAngleRoll <= 10 && KalmanAngleRoll >= -10) {

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    display.println(" Safe \n Angle...");

    Serial.println(" Safe Angle...");
    display.display();
    delay(200);
}

  while (micros() - LoopTimer < 4000);
  LoopTimer = micros();
}