//Moon phase calculation credits to:
//https://how2electronics.com/moon-phase-calculator-with-oled-display/

//Neopixel library
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(30, 10);

//Neopixel matrix
#include <Adafruit_NeoMatrix.h>
#include <gamma.h>
#include <Adafruit_GFX.h>

//RTC libraries
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 RTC;
char t[32];

//Variables for Moon Calculation
String nfm = "";  //days to next full moon

//Variable for finite state machine
int mp;
int state = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(255);

  //RTC Setup
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  stateSwap();

  Serial.println(state);
  //Serial.println(state);
  DateTime now = RTC.now();
  sprintf(t, "%02d:%02d:%02d %02d/%02d/%02d", now.hour(), now.minute(), now.second(), now.day(), now.month(), now.year());
  //Serial.print(F("Date/Time: "));
  //Serial.println(t);
  //Serial.println(now.day());
  mp = moonPhaseCalculator();
  strip.clear();

  //States controlling the colors
  switch (state) {  //change to var state for demo
      //FULL MOON (Rises at 6PM. Sets at 6AM) Fifth Phase
    case 0:
      //Serial.println("First Case!!");
      for (int index = 0; index < strip.numPixels(); index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //WANING GIBBOUS (Rises at 9PM. Sets at 9AM) Sixth Phase
    case 1:
      // Serial.println("Second Case!!");
      for (int index = 0; index < 10; index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //THIRD QUARTER (Rises at 12AM. Sets at 12PM) Seventh Phase
    case 2:
      // Serial.println("Third Case!!");
      for (int index = 0; index < 15; index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //WANING CRESCENT (Rises at 3AM. Sets at 3PM) Eighth Phase
    case 3:
      // Serial.println("Fourth Case!!");
      for (int index = 0; index < 5; index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //NEW MOON (Rises at 6AM. Sets at 6PM) First Phase
    case 4:
      // Serial.println("Fifth Case!!");
      for (int index = 0; index < strip.numPixels(); index++) {
        strip.setPixelColor(index, strip.Color(0, 0, 0));
      }
      strip.show();
      break;

      //WAXING CRESCENT (Rises at 9AM. Sets at 9PM) Second Phase
    case 5:
      //  Serial.println("Sixth Case!!");
      for (int index = 25; index < strip.numPixels(); index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //FIRST QUARTER (Rises at 12PM. Sets at 12AM) Third Phase
    case 6:
      //  Serial.println("Seventh Case!!");
      for (int index = 15; index < strip.numPixels(); index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break;

      //WAXING GIBBOUS (Rises at 3PM. Sets at 3AM) Fourth Phase
    case 7:
      //  Serial.println("Eighth Case!!");
      for (int index = 5; index < strip.numPixels(); index++) {
        strip.setPixelColor(index, strip.Color(255, 255, 255));
      }
      strip.show();
      break; 
  }
}

void stateSwap() {
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  for (int index = 0; index < 7; index++) {
    if (millis() - startTime >= 2000) {
      state = (state + 1);  //change state to mp
      startTime = millis();
    }
    // Serial.println(state);
  }
  if (state > 7) {
    state = 0;
  }
}

int moonPhaseCalculator() {
  //This will calculate the age of the moon phase (0 to 7)
  // There are eight stages, 0 is a full moon and 4 is a new
  DateTime now = RTC.now();
  Serial.print(now.hour(), now.minute());
  double jd = 0;  //Julian Date
  double ed = 0;  //Dattes elapsed since start of full moon
  int moonPhase = 0;
  jd = julianDate(now.year(), now.month(), now.day());
  //Serial.println(now.day());

  //jd = julianDate(1972,1,1) //a debug, date for a new moon
  jd = int(jd);  //- 2244116.75);  //starting at Jan 1 1972

  //Serial.println(jd);
  jd /= 29.53;  //divide by the moon cycle. Represented in days
  moonPhase = jd;
  jd -= moonPhase;                         //leaves a fractional part of jd
  ed = jd * 29.53;                         // days elapsed this month
  String nfm = String((int(29.53 - ed)));  //days to next full moon
  moonPhase = jd * 8 + 0.5;
  moonPhase = moonPhase & 7;
  //Serial.println(moonPhase);
  return moonPhase;

  //Serial.println();
}

double julianDate(int y, int m, int d) {
  //convert a date to a Julian Date
  int mm, yy;
  double k1, k2, k3;
  double j;

  yy = y - int((12 - m) / 10);
  mm = m + 9;
  if (mm >= 12) {
    mm = mm - 12;
  }
  k1 = 365.25 * (yy + 4172);
  k2 = int((30.6001 * mm) + 0.5);
  k3 = int((((yy / 100) + 4) * 0.75) - 38);
  j = k1 + k2 + d + 59;
  j = j - k3;  // j is the Julian date at 12h UT (Universal Time)
               //Serial.println(j);
  return j;
}


