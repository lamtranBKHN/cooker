#include "setting.h"
#include "webPage.h"
#include <Arduino.h>

String getPage(int currentTemp, int desireTemp, int remainingInMin) {
  String page = "<!DOCTYPE HTML><html><head>";
  page += "<title>Happy cook</title>";
  page += "<meta name='viewport' content='width=device-width', initial-scale=1'>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; text-align: center; }</style>";
  page += "<style> body2 { background-color: #fffff; font-family: Georgia, Times, Serif; Color: #000088; text-align: center; }</style>";
  page += "<style> button { background-color: #195B6A; border: none; color: white; padding: 20px;";
  page += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}";
  page += ".button3 { background-color: #f70e0a; border: none; color: white; padding: 20px;";
  page += ".button2 {background-color: #77878A;}</style></head>";
  page += "<body><h1>Happy cook</h1>";
  page += "<h2> Have a nice day <3 </h2>";
  
  page += "<form action='/get'>";
  page += "Desire Temperature: <input type='text' name='input1'>";
  page += "<input type='submit' value='Set'>";
  page += "</form><br>";

  page += "<h3> Timer </h3>";
  page += "<form action='/get'>";
  page += "H: <input type='text' name='input2'>";
  page += "m: <input type='text' name='input3'>";
  page += "<input type='submit' value='Set'>";
  page += "</form><br>";
  
  page += "</body></html>";
//  page += "";

  return page;
}
