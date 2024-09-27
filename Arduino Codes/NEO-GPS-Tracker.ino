#include <NMEAGPS.h>
#include <U8x8lib.h>
#include <GPSport.h>
static NMEAGPS  gps;
static gps_fix  fix;

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

uint32_t timer;
bool screencleared = false;

void setup() {
  gpsPort.begin( 9600 );
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.fillDisplay();
  delay(1000);
  for (uint8_t r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.clearLine(r);
    delay(100);
  }
  delay(100);
  u8x8.println("Doing");
  delay(500);
  u8x8.println("Some");
  delay(500);
  u8x8.print("Stuff");

  timer = millis();
}

static void doSomeWork()
{
  int totalSatellites, trackedSatellites;
  totalSatellites = gps.sat_count;
  for (uint8_t i = 0; i < totalSatellites; i++) {
    if (gps.satellites[i].tracked) {
      trackedSatellites++;
    }
  }

  u8x8.inverse();

  u8x8.drawString(0, 6, "TKD");
  u8x8.drawString(5, 6, "AVL");
  u8x8.drawString(10, 6, "TME");

  u8x8.noInverse();

  enum {BufSizeTracked = 3};
  char trackedchar[BufSizeTracked];
  snprintf (trackedchar, BufSizeTracked, "%d", trackedSatellites);
  u8x8.drawString(0, 7, "  ");
  u8x8.drawString(0, 7, trackedchar);

  enum {BufSizeTotal = 3};
  char availchar[BufSizeTotal];
  snprintf (availchar, BufSizeTotal, "%d", totalSatellites);
  u8x8.drawString(5, 7, "  ");
  u8x8.drawString(5, 7, availchar);

  if (fix.valid.time) {

    enum {BufSizeTime = 3};
    int hour = fix.dateTime.hours + 2;
    int minute = fix.dateTime.minutes;

    char hourchar[BufSizeTime];
    char minutechar[BufSizeTime];
    snprintf (hourchar, BufSizeTime, "%d", hour);
    snprintf (minutechar, BufSizeTime, "%d", minute);
    if ( hour < 10 )
    {
      snprintf (hourchar, BufSizeTime, "%02d", hour);
    }
    if ( minute < 10 )
    {
      snprintf (minutechar, BufSizeTime, "%02d", minute);
    }

    u8x8.drawString(10, 7, hourchar);
    u8x8.drawString(12, 7, ":");
    u8x8.drawString(13, 7, minutechar);
  }

  if (fix.valid.location) {

    if (!screencleared)
    {
      int r;
      for ( int r = 0; r < 5; r++ )
      {
        u8x8.clearLine(r);
      }
      screencleared = true;
    }

    u8x8.inverse();
    u8x8.drawString(0, 0, "FIX");
    u8x8.drawString(0, 2, "LAT");
    u8x8.drawString(0, 4, "LNG");
    u8x8.noInverse();

    enum {BufSize = 3};
    char satchar2[BufSize];
    snprintf (satchar2, BufSize, "%d", fix.satellites);
    u8x8.drawString(4, 0, "  ");
    u8x8.drawString(4, 0, satchar2);
    char latchar[10];
    dtostrf(fix.latitude(), 3, 7, latchar);
    u8x8.drawString(4, 2, latchar);
    char longchar[10];
    dtostrf(fix.longitude(), 3, 7, longchar);
    u8x8.drawString(4, 4, longchar);

  }
}

static void GPSloop()
{
  while (gps.available( gpsPort )) {
    fix = gps.read();
    doSomeWork();
  }
}

void loop()
{
  GPSloop();
  if (millis() - timer > 5000 && !screencleared) {
    timer = millis();
    u8x8.print(".");
  }
}
