#include <LiquidCrystal_I2C.h>
#include <math.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

unsigned int RS = 150000;
double Vcc = 3.3;

int desireTemp = 50;
int timeSleep = 0;
int runningMode = 0;
int runningTime = 0;

const int relayCtlPin = 14; // D5
const int menuPin = 12; // D6
const int plusPin = 13; // D7
const int minusPin = 15; // D5

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
  double R_NTC = (RS * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  Temp = Temp - 273.15;         
  return Temp;
}

void Greeting() {
  lcd.setCursor(0, 0);
  lcd.print("Welcome sweetheart!");
  lcd.setCursor(0,1);
  lcd.print("Have a nice day <3");
  delay(3000);
  lcd.clear();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  pinMode(relayCtlPin, OUTPUT);
  pinMode(menuPin, INPUT_PULLUP); 
  pinMode(plusPin, INPUT_PULLUP); 
  pinMode(minusPin, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(menuPin), handleMenuInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(plusPin), handlePlusInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(minusPin), handleMinusInterrupt, FALLING);
  Greeting();
}


void loop() {
  // Normal mode
  if(runningMode == 0) {
    int currentTemp = (int)Thermister(AnalogRead());
    lcd.clear();
    lcd.setCursor(0, 0);
    String displayText = "Act/Der: " + String(currentTemp) + "/" + String(desireTemp) + "°C";
    lcd.print(displayText);
    
    lcd.setCursor(0, 1);
    int remainingTimeInSec = timeSleep * 60 - runningTime;
    if(remainingTimeInSec <= 0) remainingTimeInSec = 0;
    displayText = "Remain: " + String(remainingTimeInSec) + "s";
    lcd.print(displayText);
    
    if (currentTemp < desireTemp) {
      if(runningTime < timeSleep * 60) {
        digitalWrite(relayCtlPin, HIGH);
      } else {
        digitalWrite(relayCtlPin, LOW);
      }
    } else {
      digitalWrite(relayCtlPin, LOW);
    }
    
    Serial.println(currentTemp);
    runningTime++;
    delay(1000);  
  }  
  
  // Set temp
  while(runningMode == 1) {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    String displayText = "Set desiretemp";
    lcd.print(displayText);
    
    lcd.setCursor(0, 1);
    displayText = String(desireTemp) + "°C";
    lcd.print(displayText);
  }

  // Set sleep Time
  while(runningMode == 2) {
    lcd.clear();
    
    lcd.setCursor(0, 0);
    String displayText = "Set sleep time";
    lcd.print(displayText);
    
    lcd.setCursor(0, 1);
    displayText = String(timeSleep) + "min";
    lcd.print(displayText);
  }
}

void handlePlusInterrupt() { 
    if( runningMode == 1) {
      desireTemp += 5;  
      if( desireTemp >= 105 ) desireTemp = 105;
    }
    else if( runningMode == 2) {
      timeSleep += 5; 
    }
    delay(100); 
}

void handleMinusInterrupt() { 
    if( runningMode == 1) {
      desireTemp -= 5;  
      if( desireTemp <= 0 ) desireTemp = 0;
    }
    else if( runningMode == 2) {
      timeSleep -= 5; 
      if( timeSleep <= 0 ) timeSleep = 0;
    }
    delay(100); 
}

void handleMenuInterrupt() { 
    digitalWrite(relayCtlPin, LOW);
    runningMode += 1;
    if(runningMode > 2) runningMode = 0;
    delay(100); 
}
