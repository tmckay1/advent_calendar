//ws2812b
#include <FastLED.h>
#define LED_PIN     4
#define NUM_LEDS    24
CRGB leds[NUM_LEDS];
CRGB genericDayColor = CRGB(0, 0, 255);
CRGB decemberDayColor = CRGB(255, 0, 0);
CRGB christmasDayColor  = CRGB(0, 255, 0);

//wifi
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "<SSID>";
const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;

// date
#include <NTPClient.h>
#include <TimeLib.h>

String currentDate = "";

NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(115200);
  

  // start up wifi
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  // init date
  timeClient.begin();

  // init fast led
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  //get current date and set the new leds
  timeClient.update();
  time_t estEpochTime = timeClient.getEpochTime() - 60 * 60 * 5;
  if(isDecember(estEpochTime)){
    drawDaysTillChristmas(estEpochTime);
  }else{
    drawGenericMonth();
  }

  // draw the new leds if the date changed
  String d = String(year(estEpochTime)) + "--" + String(month(estEpochTime)) + "--" + String(day(estEpochTime));
  if (d != currentDate) {
    currentDate = d;
    FastLED.show();
  } 
  delay(10000); 
}

bool isDecember(time_t estEpochTime) {
  return month(estEpochTime) == 12;
}

void drawDaysTillChristmas(time_t estEpochTime) {
  int currentDay = day(estEpochTime);
  if(currentDay < 25) {
    drawDecemberMonth(currentDay);
  }else if (currentDay == 25) {
    drawChristmasDay();
  }else {
    drawGenericMonth();
  }
}

// if a generic month set every led to the same color
void drawGenericMonth() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = genericDayColor;
  }
}

// if it is december draw days before christmas
void drawDecemberMonth(int currentDay) {
  for(int i = 0; i < currentDay; i++) {
    leds[23 - i] = decemberDayColor;
  }
}

// change all led colors on christmas day
void drawChristmasDay() {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = christmasDayColor;
  }
}
