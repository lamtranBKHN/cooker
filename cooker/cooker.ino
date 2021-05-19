#include <math.h>
#include "setting.h"
#include "display.h"
//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webPage.h"
#include <DNSServer.h>      

AsyncWebServer server(80);
// DNS server
const byte DNS_PORT = 53;
IPAddress apIP(10, 10, 10, 1); 
DNSServer dnsServer;

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
  isRunning = true;
  
  // Connect to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retry = 100;
  while ( retry >= 0 ) {
    if ( WiFi.status() == WL_CONNECTED ) break;
    delay ( 500 ); 
    Serial.print ( "." );
    retry--;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  accessIp = WiFi.localIP().toString();

  
  // Local
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(host_ssid);
  dnsServer.start(DNS_PORT, "*", apIP);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    int remainingSec = timeSleep * 60 - runningTime;
    request->send(200, "text/html", getPage(currentTemp, desireTemp, remainingSec));
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // Setting SSID 
    if (request->hasParam("inputWifiSsid") && request->hasParam("inputWifiPass")) {
      ssid = request->getParam("inputWifiSsid")->value().c_str();
      password = request->getParam("inputWifiPass")->value().c_str();
      setup();
    }

    // Setting temperature
    else if (request->hasParam("inputDesireTemperature")) {
      desireTemp = ( request->getParam("inputDesireTemperature")->value() ).toInt();
      Serial.println(desireTemp);
    }

    // Setting timer
    else if (request->hasParam("inputTimerHour") && request->hasParam("inputTimerMin")) {
      runningTime = 0;
      timeSleep = ( request->getParam("inputTimerHour")->value() ).toInt() * 60 + ( request->getParam("inputTimerMin")->value() ).toInt();
    }

    //Turn on
    else if(request->hasParam("turnOn")) {
      runningTime = 0;
      displayTurnLightOn();
      timeSleep = 0;
      desireTemp = 50;
      digitalWrite(relayCtlPin, LOW);
      isRunning = true;
    }

    // Turn Off
    else if(request->hasParam("turnOff")) {
      runningTime = 0;
      timeSleep = 0;
      digitalWrite(relayCtlPin, LOW);
      displayTurnOff();
      displayClear();
      isRunning = false;
    }
    
    int remainingSec = timeSleep * 60 - runningTime;
    request->send(200, "text/html", getPage(currentTemp, desireTemp, remainingSec));
  });
  
//  server.onNotFound(notFound) ;
  server.onNotFound([](AsyncWebServerRequest *request){
    int remainingSec = timeSleep * 60 - runningTime;
    request->send(200, "text/html", getPage(currentTemp, desireTemp, remainingSec));
  });
  server.begin();
 
}

void loop() {
  dnsServer.processNextRequest();
  if(isRunning) {
    displayClear();
    currentTemp = (int)Thermister(AnalogRead());
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
    displayText( accessIp, 0, 1);
    
    // Check running time
    if(runningTime < timeSleep * 60) {
      if (currentTemp < desireTemp) {
        digitalWrite(relayCtlPin, HIGH);
        displayTurnLightOn();
      } else {
        digitalWrite(relayCtlPin, LOW);
      }
      runningTime++;
    } else {
      digitalWrite(relayCtlPin, LOW);
    }
    delay(1000);   
  }
}
