#ifndef _DISPLAY_H
#define _DISPLAY_H
#include <LiquidCrystal_I2C.h>

void displayInit();
void displayText(String text, int row, int col);
void displayClear();
void displayTurnOff();

#endif
