#include "setting.h"
#include "display.h"

#include <LiquidCrystal_I2C.h>

// set LCD address, number of columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void displayInit() {
  lcd.init();
  lcd.backlight();
}

void displayText(String text, int row, int col) {
  lcd.setCursor(row, col);
  lcd.print(text);
}

void displayClear() {
  lcd.clear();
}

void displayTurnOff() {
  lcd.clear();
  lcd.noBacklight();
}