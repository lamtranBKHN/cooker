#include "setting.h"
#include "webPage.h"
#include <Arduino.h>

String getPage(int currentTemp, int desireTemp, int remainingInSec) {
  String page = "<!DOCTYPE HTML><html><head>";
  page += "<title>Happy cook</title>";
  page += "<meta name='viewport' content='width=device-width' initial-scale=1>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; text-align: center; }</style>";
  page += "<style> body2 { background-color: #fffff; font-family: Georgia, Times, Serif; Color: #000088; text-align: center; }</style>";
  page += "<style> button { background-color: #195B6A; border: none; color: white; padding: 20px;";
  page += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  page += ".button3 { background-color: #f70e0a; border: none; color: white; padding: 20px;";
  page += ".button2 {background-color: #77878A;}</style></head>";
  page += "<body><h1>Happy cook</h1>";

  page += "<br>";
  page += "<h3> Information </h3>";
  page += "<h4 style='text-align: center; color: black;' > Current temperature: " + String(currentTemp) + "&#8451 </h4>";
  page += "<h4 style='text-align: center; color: black;' > Desire temperature: " + String(desireTemp) + "&#8451 </h4>";
  if(remainingInSec > 3600 ) {
    page += "<h4 style='text-align: center; color: black;' > Remaning time: " + String(remainingInSec / 3600) + "h " + String(remainingInSec % 3600 / 60 + 1) + "m" + "</h4>";
  } else {
    page += "<h4 style='text-align: center; color: black;' > Remaning time: " + String(remainingInSec / 60) + "m " + String(remainingInSec % 60) + "s" + "</h4>";
  }
  
  page += "<br>";
  page += "<h3> Settings </h3>";

  page += "<form action='/get' style='text-align: center;'>";
  page += "WIFI SSID: <input type='text' name='inputWifiSsid'><br>";
  page += "WIFI Password: <input type='password' name='inputWifiPass'>";
  page += "<input type='submit' value='Set'>";
  page += "</form><br>";

  page += "<form action='/get' style='text-align: center;'>";
  page += "Desire Temperature: <input type='number' name='inputDesireTemperature' min='0' max='120'>";
  page += "<input type='submit' value='Set'>";
  page += "</form><br>";

  page += "<form action='/get' style='text-align: center;'>";
  page += "Timer: <input type='number' name='inputTimerHour' min='0' max='48'> h";
  page += "<input type='number' name='inputTimerMin' min='0' max='59'> m";
  page += "<input type='submit' value='Set'>";
  page += "</form><br>";

  page += "<form action='/get' style='text-align: center;'>";
  page += "<input type='submit' name='turnOn' value='Turn On'>";
  page += "</form><br>";

  page += "<form action='/get' style='text-align: center;'>";
  page += "<input type='submit' name='turnOff' value='Turn Off'>";
  page += "</form><br>";
  
  page += "</body></html>";

  page += "<h2> Have a nice day </h2>";
  return (page);
}
