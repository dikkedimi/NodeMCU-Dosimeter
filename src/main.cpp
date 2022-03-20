#include "main.h"

String clientId = "RadBoy";
uint8_t uptime = millis() / 1000;
// unsigned long lastReconnectAttempt = 0;
float readings[3];


String generateClientIdFromMac() // Convert the WiFi MAC address to String
{
  // if(config.useWiFi) {
    byte _mac[6];
    String _output = "";

    WiFi.macAddress(_mac);

    for (int _i = 5; _i > 0; _i--)
    {
      _output += String(_mac[_i], HEX);
    }
    Serial.println(_output);
    return _output;
  // }
}


void threadUpdateRadmonCallback() {

  if (config.debugRadmon) {
    Serial.println("\r\nUptime: " + String(uptime) + " Seconds");
  }
  if (config.useRM) {
    if (config.debugRadmon) {
      Serial.println("\r\nUpdating Radmon...");
    }
  }
  if (config.useRM) {
    //the path and file to send the data to:
    String urlGet = "/radmon.php";
    urlGet += "?function=submit&user=";
    urlGet += RadmonUser;
    urlGet += "&password=";
    urlGet += RadmonPass;
    urlGet += "&value=";
    urlGet += int(readings[1]);
    urlGet += "&unit=CPM";

    if (config.debugRadmon) {
      Serial.print(">>> Connecting to host: ");
      Serial.println(RadmonHost);
    }

    if (!clientGet.connect(RadmonHost, httpGetPort)) {
      if (config.debugRadmon) {
        Serial.print("Connection failed: ");
        Serial.println(RadmonHost);
      }

    } else {
      if (config.debugRadmon) {
        Serial.println("Sending data....");
      }
      if (config.debugRadmon) {
        Serial.print("sent: ");
        Serial.print(readings[1]);
        Serial.println(" CPM");
      }
      clientGet.println("GET " + urlGet + " HTTP/1.1");
      if (config.debugRadmon) {
        Serial.println("GET " + urlGet + " HTTP/1.1");
      }
      clientGet.print("Host: ");
      clientGet.println(RadmonHost);
      if (config.debugRadmon) {
        Serial.println("Host: " + String(RadmonHost));
      }
      clientGet.println("User-Agent: ESP8266/1.0");
      if (config.debugRadmon) {
        Serial.println("User-Agent: ESP8266/1.0");
      }
      clientGet.println("Connection: close\r\n\r\n");
      if (config.debugRadmon) {
        Serial.println("Connection: close\r\n\r\n");
      }

      unsigned long timeoutP = millis();
      while (clientGet.available() == 0) {
        if (millis() - timeoutP > 10000) {
          if (config.debugRadmon) {
            Serial.print(">>> Client Timeout: ");
            Serial.println(RadmonHost);
          }
          clientGet.stop();
          return;
        }
      }

      if (config.debugRadmon) {
        Serial.println("End of sending data....\r\n\r\nResponse:");
      }

      //just checks the 1st line of the server response. Could be expanded if needed.
      while (clientGet.available()) {
        String retLine = clientGet.readStringUntil('\r');
        if (config.debugRadmon) {
          Serial.println(retLine);
        }
        break;
      }
    } //end client connection if else

    if (config.debugRadmon) {
      Serial.print(">>> Closing host: ");
      Serial.println(RadmonHost);
    }
    clientGet.stop();
  }
}

void threadCurrentLogCallback()
{

    float _cpmMinuteAverage = 0; // dit is het average van de afgelopen minuut (MAX_PERIOD)

  // Serial.println("threadCurrentLogCallback");
  for (int _i = ENTRIES - 1; _i > 0; _i--) {
    logs[_i] = logs[_i - 1];
    _cpmMinuteAverage += logs[_i];
    if(config.debugCPMcalc) {
  //     Serial.print("_cpmTotal: ");Serial.println(_cpmTotal);
     Serial.print("_cpmMinuteAverage: ");Serial.println(_cpmMinuteAverage);Serial.print(" _i: ");Serial.println(_i);
    }
  }

  if(config.debugCPMcalc) {
    Serial.print("counts[0]");Serial.println(counts[0]);
  }

  logs[0] = counts[0];
  if(config.debugCPMcalc) {
    Serial.print("logs[0]");Serial.println(logs[0]);
  }

  counts[1] += counts[0];
  if(config.debugCPMcalc) {
    Serial.print("counts[1]");Serial.println(counts[1]);
  }

  counts[0] = 0;
  if(config.debugCPMcalc) {
    Serial.print("counts[0]");Serial.print(counts[0]);Serial.print("   counts[1]");Serial.println(counts[1]);
  }

  float _cpmTotal = (float)counts[1] / float (millis() / 1000) * 60; // dit is een inter/extrapolatie van het totaal
   if(config.debugCPMcalc) {
     Serial.print("_cpmTotal: ");Serial.println(_cpmTotal);
//     Serial.print("_cpmMinuteAverage: ");Serial.println(_cpmMinuteAverage);
    }
   float _dose = _cpmTotal / tubeIndex; // dit is de tube index CPM / 151 = uSv/h
   if(config.debugCPMcalc) {
    Serial.print("_dose: ");Serial.println(_dose);
   }
  readings[0] = _cpmTotal;
  readings[1] = _cpmMinuteAverage;
  readings[2] = _dose;
}

void threadUpdateDisplayCallback() {
  if (config.debugDisplay) {
    Serial.println("Updating Display!");
  }
  display.clearDisplay();

  if(config.useWiFi) {
    // HOSTNAME
    display.setCursor(0, 0);
    display.setFont();

    // display.setTextSize(1);
    display.println(clientId);
    display.setCursor(56, 9);
    display.print(" @ ");

    // IP ADDRESS
    display.println(WiFi.localIP());
  } else {
    display.setCursor(0, 0);
    display.setFont();
    // display.setTextSize(1);
    display.print("avg ");display.print(MAX_PERIOD);display.print("s:   ");
    // _cpmMinuteAverage
    display.print(readings[0]);display.println("CPM");
    // display.setCursor(56, 9);
    // _cpmTotal
    display.print("avg ");display.print(LOG_PERIOD);display.print("s:     ");
    display.print(readings[1]);display.println("CPM");

  }
  // LABEL
  display.setFont(&FreeSansOblique9pt7b);
  display.setTextSize(1);
  display.setCursor(0, 28);
  display.print(F("Dose:"));

  // display.print("\r\nUptime: ");display.print(uptime);

  // VALUE
  display.setCursor(0, 62);
  display.setFont(&FreeSans24pt7b);
  display.setTextSize(1); // Draw 3X-scale text
  display.print(readings[2]);

  // UNIT
  display.setCursor(97, 61);
  display.setFont();
  display.setTextSize(1); // Draw 3X-scale text
  display.print(F("mS/h"));
  display.display();      // Show initial text
}

void IRAM_ATTR countPulse()
{
  // if(PRINT_TO_SERIAL) {
  //  Serial.print("pulse detected");
  // }
  digitalWrite(LED_BUILTIN, LOW);
  counts[0]++;
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  pinMode(PulsePin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for (int _i = 0; _i < ENTRIES; _i++) {
    logs[_i] = 0;
  }

  Serial.begin(115200);
    if(PRINT_TO_SERIAL) {
  Serial.println("Setup begun.");
  }
  if(config.useWiFi) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, WiFipass);

    while(WiFi.status() != WL_CONNECTED)  {
      Serial.print(".");
      delay(500);
    }


    Serial.println();
    Serial.println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  Wire.begin(SDA, SCL);

if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, false, false)) {
  Serial.println(F("SSD1306 allocation failed"));
  // for(;;);
}
  display.ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  display.ssd1306_command(0x8F);
  display.clearDisplay();
  clientId += "-" + generateClientIdFromMac();

  threadCurrentLog.enabled = true;
  threadCurrentLog.setInterval(LOG_PERIOD * 1000);
  threadCurrentLog.onRun(threadCurrentLogCallback);
if(config.useWiFi) {
  threadUpdateRadmon.enabled = false;
  // threadUpdateRadmon.setInterval(RadmonUpdateInterval * 1000);
  // threadUpdateRadmon.onRun(threadUpdateRadmonCallback);
} else {
  threadUpdateRadmon.enabled = true;
  threadUpdateRadmon.setInterval(RadmonUpdateInterval * 1000);
  threadUpdateRadmon.onRun(threadUpdateRadmonCallback);
}

  threadUpdateDisplay.enabled = true;
  threadUpdateDisplay.setInterval(DisplayUpdateInterval * 1000);
  threadUpdateDisplay.onRun(threadUpdateDisplayCallback);

  attachInterrupt(digitalPinToInterrupt(PulsePin), countPulse, FALLING);
  display.setTextSize(1); // Draw 1X-scale text
  display.setTextColor(SSD1306_WHITE);

  if(config.debugStartup) {
    Serial.println("Setup done.");
  }
}

void loop() {
// Run threads, this makes it all work on time!
  threadController.run();
}