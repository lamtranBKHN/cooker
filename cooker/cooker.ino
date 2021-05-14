#include <math.h>
#include "setting.h"
#include "display.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

int AnalogRead() {
  int val = 0;
  for(int i = 0; i < 20; i++) {
    val += analogRead(A0);
    delay(1);
  }
  val = val / 20;
  return val;
}

double Thermister(int val) {
  double V_NTC = (double)val / 1024;
  double R_NTC = (2.7 * RS * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  Temp = Temp - 273.15;         
  return Temp;
}

void Greeting() {
  displayText("Love You!", 3, 0);
  displayText("Have a nice day <3", 0, 1);
}

void setup() {
  displayInit();
  Greeting();
  Serial.begin(115200);
  pinMode(relayCtlPin, OUTPUT);

  // Connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retry = 10;
  while ( retry-- ) {
    if ( WiFi.status() != WL_CONNECTED ) break;
    delay ( 500 ); 
    Serial.print ( "." );
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Local
  WiFi.softAP(host_ssid, host_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);

  
}


void loop() {
  displayClear();
  int currentTemp = (int)Thermister(AnalogRead());
  int remainingSec = timeSleep * 60 - runningTime;
  // Remaining time is over 1h
  if( remainingSec > 3600) {
    int remainingTimeInHour = (int)( remainingSec / 3600 );
    int remainingTimeInMin  = (int)( (int)( remainingSec % 3600 ) / 60 + 1);
    String displayTextWrite = String(currentTemp) + "/" + String(desireTemp) + "\337C " + String(remainingTimeInHour) + "h" + String(remainingTimeInMin) + "m";
    displayText(displayTextWrite, 0, 0);
  } else {
    int remainingTimeInMin = (int)( remainingSec / 60 );
    int remainingTimeInSec  = (int)( remainingSec % 60 );
    String displayTextWrite = String(currentTemp) + "/" + String(desireTemp) + "\337C " + String(remainingTimeInMin) + "m" + String(remainingTimeInSec) + "s";
    displayText(displayTextWrite, 0, 0);
  }

  // Display IP
  displayText( WiFi.localIP().toString(), 1, 0);
  
  // Check running time
  if(runningTime < timeSleep * 60) {
    if (currentTemp < desireTemp) {
      digitalWrite(relayCtlPin, HIGH);
    } else {
      digitalWrite(relayCtlPin, LOW);
    }
    runningTime++;
  } else {
    digitalWrite(relayCtlPin, LOW);
    displayTurnOff();
  }
  
  Serial.println(currentTemp);
  delay(1000);  
}
