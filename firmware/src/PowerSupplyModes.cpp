#include "PowerSupplyModes.h"

#include "Constants.h"
#include "DisplayDriver.h"
#include "TunixMemoryManager.h"
#include "TempController.h"
#include "DisplayDriver.h"
#include "OutputManager.h"

byte questVoltChangeCursor;

PowerSupplyClass::PowerSupplyClass() { }

int PowerSupplyClass::standbyMode()
{
  digitalWrite(pins::POWER_CONTROL, LOW);
  digitalWrite(pins::VOLTAGE_CONTROL, LOW);
  for (int counter = 255; counter != 0; counter--) 
  {
    analogWrite(pins::SCREEN_BACKLIGHT, counter);
    delay(5);
  }
  display.clearScreen();
  while(digitalRead(pins::TOP_BUTTON) == HIGH)
  {
    delay(2000);
    temperature.tempControl();
  }
  digitalWrite(pins::SCREEN_BACKLIGHT, HIGH);
  digitalWrite(pins::POWER_CONTROL, HIGH);
  digitalWrite(pins::VOLTAGE_CONTROL, HIGH);
  tone(pins::BUZZER, 1800 , 50);
  return 0;
}

void PowerSupplyClass::basicMod()
{
  digitalWrite(pins::RELAY, LOW);
  display.relayPosition = 0;
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
  digitalWrite(pins::RELAY, LOW);
  display.relayPosition = false;
  while(digitalRead(pins::LEFT_BUTTON) == LOW);
  tone(pins::BUZZER, 1800, 50);
  return;
}

void PowerSupplyClass::questVoltChangeScreenLines(char *questVoltStr)
{
  display.textToScreenFull(31, 60, ST7735_WHITE, ST7735_BLACK, 2, questVoltStr, false);
  display.textToScreenFast(1, 1, "V", true);
  if(questVoltChangeCursor == 1)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(43, 75, 52, 75, TFT_GRAY);
    else                          display.lineToScreen(31, 75, 40, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 2)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(67, 75, 76, 75, TFT_GRAY);
    else                          display.lineToScreen(55, 75, 64, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 3)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(79, 75, 88, 75, TFT_GRAY);
    else                          display.lineToScreen(67, 75, 76, 75, TFT_GRAY);
  }
}

void PowerSupplyClass::questVoltChange()
{
  display.clearScreen();
  display.textToScreenFull(20, 5, ST7735_WHITE, ST7735_BLACK, 2, "Quest V", false);
  display.lineToScreen(0, 25, 128, 25, TFT_GRAY);
  questVoltChangeCursor = 1;
  delay(100);
  char questVoltStr[10];
  dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
  questVoltChangeScreenLines(questVoltStr);
  while(questVoltChangeCursor <= 3)
  {
    if(digitalRead(pins::TOP_BUTTON) == LOW) 
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::TOP_BUTTON) == LOW);
      questVoltChangeCursor++;
      questVoltChangeScreenLines(questVoltStr);
    }
    while(digitalRead(pins::LEFT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      if(questVoltChangeCursor == 1) memory.settings.questVolt -= 1;
      if(questVoltChangeCursor == 2) memory.settings.questVolt -= 0.1;
      if(questVoltChangeCursor == 3) memory.settings.questVolt -= 0.01;
      memory.settings.questVolt = constrain(memory.settings.questVolt, 0, characteristics::MAX_VOLTAGE);
      dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
    while(digitalRead(pins::RIGHT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      if(questVoltChangeCursor == 1) memory.settings.questVolt += 1;
      if(questVoltChangeCursor == 2) memory.settings.questVolt += 0.1;
      if(questVoltChangeCursor == 3) memory.settings.questVolt += 0.01;
      memory.settings.questVolt = constrain(memory.settings.questVolt, 0, characteristics::MAX_VOLTAGE);
      dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
  }
  questVoltChangeCursor = 1;
  EEPROM.put(SETTINGS_ADRESS, memory.settings);
  display.firstScreenWrite = 1;
  tone(pins::BUZZER, 1800, 50);
  return;
}

void PowerSupplyClass::advancedMod()
{
  digitalWrite(pins::RELAY, LOW);
  display.relayPosition = 0;
  display.firstScreenWrite = 1;
  /*display.setCursor(87, 90);
  display.setTextSize(1);
  if(memory.settings.currentProtectionMode == 1)
  {
    display.print("CC");
  }
  if(memory.settings.currentProtectionMode == 0)
  {
    display.print("CP");
  }*/
  display.lineToScreen(82, 27, 128, 27, TFT_GRAY);
  display.lineToScreen(82, 44, 128, 44, TFT_GRAY);
  while(digitalRead(pins::TOP_BUTTON) == LOW);
  display.powerSupplyModeLines();
  previousTime = currentTime;
  secondaryPreviousTime = currentTime;
  while(digitalRead(pins::LEFT_BUTTON) == HIGH)
  {
    currentTime = millis();
    display.voltCalculated = output.readVoltage();
    display.voltMainCalculated = output.readMainVoltage();
    display.currentCalculated = output.readCurrent();
    if(digitalRead(pins::RIGHT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      digitalWrite(pins::RELAY, LOW);
      display.relayPosition = pins::RELAY;
      questVoltChange();
      display.powerSupplyModeLines();
    }
    if(digitalRead(pins::TOP_BUTTON) == LOW)
    {
      display.relayPosition = pins::RELAY;
      output.setVoltage(memory.settings.questVolt);
    }
    if(currentTime - previousTime >= refreshTime)
    {
      //Main Manager
      previousTime = currentTime;
      display.advancedModScreenManager(display.voltCalculated, display.currentCalculated);
      display.firstScreenWrite = 0;
    }
    if(currentTime - secondaryPreviousTime >= secondaryRefreshTime)
    {
      //Secondary Manager
      secondaryPreviousTime = currentTime;
      temperature.tempControl();
    }
  }
  digitalWrite(pins::RELAY, LOW);
  display.relayPosition = false;
  while(digitalRead(pins::LEFT_BUTTON) == LOW);
  display.clearScreen(ST7735_BLACK);
  tone(pins::BUZZER, 1800, 50);
  return;
}