#include "LiquidCrystalI2C.h"
#include <Wire.h>

LiquidCrystalI2C lcd;

void setup()
{
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("SimpleDriver");
  lcd.setCursor(0, 1);
  lcd.print(" Test 0123456789");
  
  pinMode(13, OUTPUT);
}

void loop()
{
  lcd.setCursor(5, 1);
  lcd.print("-");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second

  lcd.setCursor(5, 1);
  lcd.print("|");
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500);               // wait for a second
}
