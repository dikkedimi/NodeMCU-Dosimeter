#include "config.h"
#include <Arduino.h>

#include <ESP8266WiFi.h> // only needed for nodemcu
#include <ESP8266HTTPClient.h> // only needed for nodemcu

#include <WiFiClient.h>
// #include <NTPClient.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBoldOblique18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeSansOblique12pt7b.h>
#include <Fonts/FreeSansOblique18pt7b.h>
#include <Fonts/FreeSansOblique24pt7b.h>

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>

#include <Thread.h>
#include <StaticThreadController.h>

unsigned long counts[2] = {0, 0}, logs[ENTRIES];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// WiFiServer server(1883);
// WiFiClient wificlient;
WiFiClient clientGet;
const int httpGetPort = 80;
const uint8_t buzzer = 15; //buzzer to arduino pin 9

Thread threadCurrentLog = Thread();
Thread threadUpdateRadmon = Thread();
Thread threadUpdateDisplay = Thread();
Thread threadAlarm = Thread();
StaticThreadController<4> threadController (&threadCurrentLog, &threadUpdateRadmon, &threadUpdateDisplay, &threadAlarm);
