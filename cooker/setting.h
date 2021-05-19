#ifndef _SETTING_H
#define _SETTING_H

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
extern int lcdColumns;
extern int lcdRows;

extern unsigned int RS;
extern double Vcc;

extern int desireTemp;
extern int timeSleep;
extern int currentTemp;
extern int runningMode;
extern int runningTime;

extern const int relayCtlPin;

extern const char* ssid;
extern const char* password;

extern const char* host_ssid;
extern const char* host_password;

extern String accessIp;
extern bool isRunning;

#endif
