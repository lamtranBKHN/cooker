#include "setting.h"
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

unsigned int RS = 100000;
double Vcc = 3.3;

int desireTemp = 50;
int timeSleep = 0;
int currentTemp = 0;
int runningTime = 0;

const int relayCtlPin = 12; // D5

const char* ssid = "TANG 1";
const char* password = "0973387991";

const char* host_ssid = "Happy Cooker";
const char* host_password = "12345678";

String accessIp = "";
bool isRunning = false;
