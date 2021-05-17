#include <math.h>
#include "setting.h"
#include "display.h"
//#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webPage.h"

AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "inputWifiSsid";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

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
//  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int retry = 100;
  while ( retry >= 0 ) {
    if ( WiFi.status() != WL_CONNECTED ) break;
    delay ( 500 ); 
    Serial.print ( "." );
    retry--;
  }
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Local
  WiFi.softAP(host_ssid, host_password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println(IP);

  /////////////////////////
  
  
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", getPage(0, 0, 0));
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    if (request->hasParam("inputWifiSsid") || request->hasParam("inputWifiPass")) {
      String ssid = 
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      
      char* ssid = "ThienTuyenTu";
      char* password = "12041998";
      Serial.println(PARAM_INPUT_3);
      setup();
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
  
  
  ///////////////////////////  
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
  displayText( WiFi.localIP().toString(), 0, 1);
//  displayText( WiFi.softAPIP().toString(), 0, 1);
  
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
