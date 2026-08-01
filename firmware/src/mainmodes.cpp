#include "Adafruit_ST7735.h"
#include "Arduino.h"
#include "mainmodes.h"
#include "constants.h"
#include "variables.h"
#include "display.h"
#include "tempcontroller.h"
#include "poweroutputmanager.h"
#include "secondaryfunctions.h"
#include "memorymanager.h"

byte questVoltChangeCursor;

int standbyMode()
{
  digitalWrite(powerControlPin, LOW);
  digitalWrite(voltageControlPin, LOW);
  for (int counter = 255; counter != 0; counter--) 
  {
    analogWrite(screenBacklightPin, counter);
    delay(5);
  }
  tft.fillScreen(ST7735_BLACK);
  while(digitalRead(topButtonPin) == HIGH)
  {
    delay(2000);
    tempControl();
  }
  digitalWrite(screenBacklightPin, HIGH);
  digitalWrite(powerControlPin, HIGH);
  digitalWrite(voltageControlPin, HIGH);
  tone(buzzerPin, 1800 , 50);
  return 0;
}

void basicMod()
{
  digitalWrite(relayPin, LOW);
  relayPosition = 0;
  previousTime = currentTime;
  secondaryPreviousTime = currentTime;
  /*basicModLines();
  while(digitalRead(topButtonPin) == HIGH);
  while(digitalRead(leftButtonPin) == LOW)
  {
    currentTime = millis();
    voltControl();
    potValue = analogRead(potPin);
    pwmValue = map(potValue, 0, 1023, 0, 253);
    analogWrite(voltageControlPin, pwmValue);
    if(digitalRead(rightButtonPin) == HIGH) 
    {
      relayChange();
    }
    if(current > settings.currentLimit)
    {
      delay(50);
      if(current > settings.currentLimit) relayOff();
    }
    if(currentTime - previousTime >= refreshTime)
    {
      previousTime = currentTime;
      basicModScreenManager(voltCalculated, currentCalculated);
      tempControl();
    }
  }*/
  digitalWrite(relayPin, LOW);
  relayPosition = false;
  while(digitalRead(leftButtonPin) == LOW);
  tone(buzzerPin, 1800, 50);
  return;
}

void questVoltChangeScreenLines(char *questVoltStr)
{
  textToScreenFull(31, 60, ST7735_WHITE, ST7735_BLACK, 2, questVoltStr, false);
  textToScreenFast(1, 1, "V", true);
  if(questVoltChangeCursor == 1)
  {
    if(settings.questVolt >= 10)  lineToScreen(43, 75, 52, 75, TFT_GRAY);
    else                          lineToScreen(31, 75, 40, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 2)
  {
    if(settings.questVolt >= 10)  lineToScreen(67, 75, 76, 75, TFT_GRAY);
    else                          lineToScreen(55, 75, 64, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 3)
  {
    if(settings.questVolt >= 10)  lineToScreen(79, 75, 88, 75, TFT_GRAY);
    else                          lineToScreen(67, 75, 76, 75, TFT_GRAY);
  }
}

void questVoltChange()
{
  tft.fillScreen(ST7735_BLACK);
  textToScreenFull(20, 5, ST7735_WHITE, ST7735_BLACK, 2, "Quest V", false);
  lineToScreen(0, 25, 128, 25, TFT_GRAY);
  questVoltChangeCursor = 1;
  delay(100);
  char questVoltStr[10];
  dtostrf(settings.questVolt, 5, 3, questVoltStr);
  questVoltChangeScreenLines(questVoltStr);
  while(questVoltChangeCursor <= 3)
  {
    if(digitalRead(topButtonPin) == LOW) 
    {
      tone(buzzerPin, 1500, 20);
      while(digitalRead(topButtonPin) == LOW);
      questVoltChangeCursor++;
      questVoltChangeScreenLines(questVoltStr);
    }
    while(digitalRead(leftButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      if(questVoltChangeCursor == 1) settings.questVolt -= 1;
      if(questVoltChangeCursor == 2) settings.questVolt -= 0.1;
      if(questVoltChangeCursor == 3) settings.questVolt -= 0.01;
      settings.questVolt = constrain(settings.questVolt, 0, MAX_VOLTAGE);
      dtostrf(settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
    while(digitalRead(rightButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      if(questVoltChangeCursor == 1) settings.questVolt += 1;
      if(questVoltChangeCursor == 2) settings.questVolt += 0.1;
      if(questVoltChangeCursor == 3) settings.questVolt += 0.01;
      settings.questVolt = constrain(settings.questVolt, 0, MAX_VOLTAGE);
      dtostrf(settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
  }
  questVoltChangeCursor = 1;
  EEPROM.put(settingsAdress, settings);
  firstScreenWrite = 1;
  tone(buzzerPin, 1800, 50);
  return;
}

void advancedMod()
{
  digitalWrite(relayPin, LOW);
  relayPosition = 0;
  firstScreenWrite = 1;
  /*tft.setCursor(87, 90);
  tft.setTextSize(1);
  if(settings.currentProtectionMode == 1)
  {
    tft.print("CC");
  }
  if(settings.currentProtectionMode == 0)
  {
    tft.print("CP");
  }*/
  lineToScreen(82, 27, 128, 27, TFT_GRAY);
  lineToScreen(82, 44, 128, 44, TFT_GRAY);
  while(digitalRead(topButtonPin) == LOW);
  powerSupplyModeLines();
  previousTime = currentTime;
  secondaryPreviousTime = currentTime;
  while(digitalRead(leftButtonPin) == HIGH)
  {
    currentTime = millis();
    voltCalculated = readVoltage();
    voltMainCalculated = readMainVoltage();
    currentCalculated = readCurrent();
    if(digitalRead(rightButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      digitalWrite(relayPin, LOW);
      relayPosition = relayPin;
      questVoltChange();
      powerSupplyModeLines();
    }
    if(digitalRead(topButtonPin) == LOW)
    {
      relayChange();
      setVoltage(settings.questVolt);
    }
    if(currentTime - previousTime >= refreshTime)
    {
      //Main Manager
      previousTime = currentTime;
      advancedModScreenManager(voltCalculated, currentCalculated);
      firstScreenWrite = 0;
    }
    if(currentTime - secondaryPreviousTime >= secondaryRefreshTime)
    {
      //Secondary Manager
      secondaryPreviousTime = currentTime;
      tempControl();
    }
  }
  digitalWrite(relayPin, LOW);
  relayPosition = false;
  while(digitalRead(leftButtonPin) == LOW);
  tft.fillScreen(ST7735_BLACK);
  tone(buzzerPin, 1800, 50);
  return;
}