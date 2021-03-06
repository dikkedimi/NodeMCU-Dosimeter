#define TIMEZONE 2

// Needed for Nodemcu v2.0
// #define SDA 4
// #define SCL 5
#define SCREEN_ADDRESS 0x3C //
#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define Interval 1500
#define tubeIndex 151

#define PulsePin 13 //for esp12 use pin 13 (D7)
#define LedPin 2 // doesn't do anything, should it be pin 0?

#define LOG_PERIOD 10 // in seconds
#define MAX_PERIOD 60 // in seconds
#define ENTRIES (MAX_PERIOD / LOG_PERIOD)

#define RadmonUpdateInterval 10
#define DisplayUpdateInterval 1

// hardware & services config
#define SEND_TO_RADMON      false        // CPM will be sent to Radmon.org
#define PRINT_TO_SERIAL     true
#define ENABLE_WIFI         false

//Debug switches
#define debug_Display            false
#define debug_Radmon             false
#define debug_CPMcalc            true
#define debug_Startup            true

const char* ssid     = "SSID";
const char* WiFipass = "PSK";

const char *time_server = "nl.pool.ntp.org";

const char *RadmonUser = "USER";
const char *RadmonPass = "PASS";
const char* RadmonHost = "radmon.org"; // No need to change this

struct strConfig {
  bool useRM = SEND_TO_RADMON;
  bool useWiFi = ENABLE_WIFI;
  // bool useBL = SEND_TO_BLYNK;
  // bool useHT = SEND_TO_HTTP;
  bool cpmSerialOn = PRINT_TO_SERIAL;
  // bool useBME280 = USE_BME280;
  // bool adr77 = USE_ADR77;
  // bool useBAR = USE_BAR;
  // bool useCelsius = CELSIUS;
  bool debugStartup = debug_Startup;
  bool debugDisplay = debug_Display;
  bool debugRadmon = debug_Radmon;
  // bool debugBlynk = debug_Blynk;
  // bool debugHTTP = debug_HTTP;
  // bool debugBME280 = debug_BME280;
  bool debugCPMcalc = debug_CPMcalc;
  // bool debugButtons = debug_Buttons;
  // long elevation = ELEVATION;
  ;
}   config;
