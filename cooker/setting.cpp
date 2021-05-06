#include "setting.h"

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

unsigned int RS = 100000;
double Vcc = 3.3;

int desireTemp = 50;
int timeSleep = 1;
int runningMode = 0;
int runningTime = 0;

const int relayCtlPin = 12; // D5