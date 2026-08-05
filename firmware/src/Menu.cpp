#include "Menu.h"
#include "DisplayDriver.h"
#include "Constants.h"
#include "DisplayDriver.h"
#include "TempController.h"
#include "PowerSupplyModes.h"
#include "TunixMemoryManager.h"

MenuClass::MenuClass() 
{ 
  menuCursor = 1;
}

void MenuClass::tunixBadge()
{
  display.textToScreenFull(22, 3, TFT_GRAY, ST7735_BLACK, 1, F("T"), false);
  display.textToScreenFast(42, 3, F("U"), false);
  display.textToScreenFast(62, 3, F("N"), false);
  display.textToScreenFast(82, 3, F("I"), false);
  display.textToScreenFast(102, 3, F("X"), false);
  display.lineToScreen(0, 14, 128, 14, TFT_GRAY);
}

void MenuClass::menuGridLines()
{
  display.lineToScreen(12, 34, 116, 34, TFT_GRAY);
  display.lineToScreen(12, 51, 116, 51, TFT_GRAY);
  display.lineToScreen(12, 68, 116, 68, TFT_GRAY);
  display.lineToScreen(12, 85, 116, 85, TFT_GRAY);
  display.lineToScreen(12, 102, 116, 102, TFT_GRAY);
}

void MenuClass::settingsMenuTextData()
{
  char currentLimitStr[10];
  dtostrf(memory.settings.currentLimit, 5, 2, currentLimitStr);
  if(memory.settings.currentLimit >= 10) display.textToScreenFull(94, 40, ST7735_WHITE, ST7735_BLACK, 1, currentLimitStr, false);
  else display.textToScreenFull(100, 40, ST7735_WHITE, ST7735_BLACK, 1, currentLimitStr, false);

  if(memory.settings.currentProtectionMode == true) display.textToScreenFast(112, 57, F("CC"), false);
  if(memory.settings.currentProtectionMode == false) display.textToScreenFast(112, 57, F("CP"), false);

  if(memory.settings.batteryMode == true) display.textToScreenFast(94, 74, F("Active"), false); 
  if(memory.settings.batteryMode == false) display.textToScreenFast(92, 74, F("Passive"), false);
}

void MenuClass::settingsMenuText()
{
  display.textToScreenFull(5, 23, ST7735_WHITE, ST7735_BLACK, 1, F("Volt Tolerance"), false);
  display.textToScreenFast(5, 40, F("Current Limitation"), false);
  display.textToScreenFast(5, 74, F("Battery Mode"), false);
  display.textToScreenFast(5, 91, F("Factory Reset"), false);
  display.textToScreenFast(5, 108, F("Back"), false);
}



void MenuClass::mainMenuText()
{
  display.textToScreenFull(34, 23, ST7735_WHITE, ST7735_BLACK, 1 , F("Basic Mode"), false);
  display.textToScreenFast(25, 40, F("Advanced Mode"), false);
  display.textToScreenFast(40, 57, F("Settings"), false);
  display.textToScreenFast(31, 74, F("Information"), false);
}

void MenuClass::menuSelectionCursor()
{
  short cursorXPosition;
  for(short g = 20; g < 106; g = g + 17) display.drawCircleShape(2, g, 123, 13, ST7735_BLACK);
  if(menuCursor == 1) cursorXPosition = 20;
  if(menuCursor == 2) cursorXPosition = 37;
  if(menuCursor == 3) cursorXPosition = 54;
  if(menuCursor == 4) cursorXPosition = 71;
  if(menuCursor == 5) cursorXPosition = 88;
  if(menuCursor == 6) cursorXPosition = 105;
  display.drawCircleShape(2, cursorXPosition, 123, 13, ST7735_WHITE);
  return;
}

void MenuClass::infoMenu()
{
  short textPos, textCount = 1;
  display.clearScreen();
  display.textToScreenFull(16, 5, TFT_GRAY, ST7735_BLACK, 1, F("Tunix Electronics"), false);
  display.lineToScreen(2, 14, 126, 14, TFT_GRAY);
  for (textCount = 17; textPos < 128; textPos = textPos + 10)
  {
    switch(textCount)
    {
      case 1: display.textToScreenFull(3, textPos, TFT_GRAY, ST7735_BLACK, 1, F("MCU"), false); break;
      case 2: display.textToScreenFast(3, textPos, F("MHZ"), false); break;
      case 3: display.textToScreenFast(3, textPos, F("GPU"), false); break;
      case 4: display.textToScreenFast(3, textPos, F("GFX.API"), false); break;
      case 5: display.textToScreenFast(3, textPos, F("ROM"), false); break;
      case 6: display.textToScreenFast(3, textPos, F("RAM"), false); break;
      case 7: display.textToScreenFast(3, textPos, F("OS"), false); break;
      case 8: display.textToScreenFast(3, textPos, F("OS VER"), false); break;
      case 9: display.textToScreenFast(3, textPos, F("BUILD"), false); break;
      case 10: display.textToScreenFast(3, textPos, F("VOLT LIM"), false); break;
      case 11: display.textToScreenFast(3, textPos, F("SCREEN"), false); break;
    }
    textCount++;
  }
  textCount = 1;
  for (textPos = 17; textPos < 128; textPos = textPos + 10) 
  {
    switch(textCount)
    {
      case 1: display.textToScreenFull(60, textPos, ST7735_WHITE, ST7735_BLACK, 1, F("Atmega328P"), false); break;
      case 2: display.textToScreenFast(3, textPos, F("16MHZ"), false); break;
      case 3: display.textToScreenFast(3, textPos, F("ST7735"), false); break;
      case 4: display.textToScreenFast(3, textPos, F("ADAFRUIT"), false); break;
      case 5: display.textToScreenFast(3, textPos, F("NaN/30720"), false); break;
      case 6: display.textToScreenFast(3, textPos, F("NaN/2048"), false); break;
      case 7: display.textToScreenFast(3, textPos, F("Tunix PS.UI"), false); break;
      case 8: display.textToScreenFast(3, textPos, F("V0.6.0"), false); break;
      case 9: display.textToScreenFast(3, textPos, F("26.08.A"), false); break;
      case 10: display.textToScreenFast(3, textPos, F("0-24V"), false); break;
      case 11: display.textToScreenFast(3, textPos, F("128x128 SPI"), false); break;
    }
    textCount++;
  }
  textCount = 1;
  while(digitalRead(pins::LEFT_BUTTON) == HIGH)
  {
    delay(2);
    sleepTime++;
    if(sleepTime >= 30000) powerSupply.standbyMode();
  }
  while(digitalRead(pins::UP_BUTTON) == LOW);
  tone(pins::BUZZER, 1800, 50);
  return;
}

void MenuClass::settingsMenu()
{
  display.clearScreen();
  menuCursor = 1;
  tunixBadge();
  menuGridLines();
  settingsMenuText();
  settingsMenuTextData();
  menuSelectionCursor();
  while(true)
  {
    if(digitalRead(pins::UP_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::UP_BUTTON) == LOW);
      menuCursor--;
      menuCursor = constrain(menuCursor, 1, 6);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(pins::DOWN_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::DOWN_BUTTON) == LOW);
      menuCursor++;
      menuCursor = constrain(menuCursor, 1, 6);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(pins::RIGHT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1800, 60);
      menuSelectionCursor();
      while(digitalRead(pins::RIGHT_BUTTON) == LOW);
      
      if(menuCursor == 2)
      {
        short buttonHoldTime, flexibleDelay;
        while (digitalRead(pins::LEFT_BUTTON) == HIGH)
        {
          buttonHoldTime = 0;
          flexibleDelay = 200;
          returnTime++;
          delay(2);
          if(returnTime >= 7500) break;
          while (digitalRead(pins::UP_BUTTON) == LOW)
          {
            tone(pins::BUZZER, 1200, 20);
            buttonHoldTime++;
            memory.settings.currentLimit =+ 0.1;
            memory.settings.currentLimit = constrain(memory.settings.currentLimit, 0.1, 20.0);
            if(buttonHoldTime > 10) flexibleDelay = 100;
            if(buttonHoldTime > 25) flexibleDelay = 75;
            returnTime = 0;
            settingsMenuTextData();
            delay(flexibleDelay);
          }
          while (digitalRead(pins::DOWN_BUTTON) == LOW)
          {
            tone(pins::BUZZER, 1200, 20);
            buttonHoldTime++;
            memory.settings.currentLimit =- 0.1;
            memory.settings.currentLimit = constrain(memory.settings.currentLimit, 0.1, 20.0);
            if(buttonHoldTime > 10) flexibleDelay = 100;
            if(buttonHoldTime > 25) flexibleDelay = 75;
            returnTime = 0;
            settingsMenuTextData();
            delay(flexibleDelay);
          }
        }
        tone(pins::BUZZER, 1800, 60);
        returnTime = 0;
        while(digitalRead(pins::LEFT_BUTTON) == LOW);
      }
      if(menuCursor == 3)
      {
        while(digitalRead(pins::RIGHT_BUTTON) == LOW);
        memory.settings.currentProtectionMode = !memory.settings.currentProtectionMode;
        settingsMenuTextData();
      }
      if(menuCursor == 4)
      {
        while(digitalRead(pins::RIGHT_BUTTON) == LOW);
        memory.settings.batteryMode = !memory.settings.batteryMode;
        settingsMenuTextData();
      }
      if(menuCursor == 5)
      {
        while(digitalRead(pins::RIGHT_BUTTON) == LOW);
        memory.factoryReset();
      }
      if(menuCursor == 6)
      {
        while(digitalRead(pins::RIGHT_BUTTON) == LOW);
        menuCursor = 1;
        return;
      }
    }
    sleepTime++;
    if(sleepTime >= 30000) sleepTime = powerSupply.standbyMode();
    delay(2);
  }
  sleepTime = 0;
  return;
}


void MenuClass::mainMenu()
{
  while(digitalRead(pins::LEFT_BUTTON) == LOW);
  display.clearScreen();
  menuCursor = 1;
  bool drawMenu = true;
  int tempControlTime;
  while(true)
  {
    if(drawMenu == true)
    {
      display.clearScreen();
      tunixBadge();
      menuGridLines();
      mainMenuText();
      menuSelectionCursor();
      drawMenu = false;
    }
    if(digitalRead(pins::UP_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::UP_BUTTON) == LOW);
      menuCursor--;
      menuCursor = constrain(menuCursor, 1, 4);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(pins::DOWN_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::DOWN_BUTTON) == LOW);
      menuCursor++;
      menuCursor = constrain(menuCursor, 1, 4);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(pins::RIGHT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1800, 50);
      sleepTime = 0;
      while(digitalRead(pins::RIGHT_BUTTON) == LOW);
      switch(menuCursor)
      {
        case 1: powerSupply.basicMod(); break;
        case 2: powerSupply.advancedMod(); break;
        case 3: settingsMenu(); break;
        case 4: infoMenu(); break;
      }
      drawMenu = true;
    }
    delay(2);
    tempControlTime++;
    sleepTime++;
    if(tempControlTime >= 500) 
    {
      temperature.tempControl();
      tempControlTime = 0;
    }
    if(sleepTime >= 30000) powerSupply.standbyMode();
  }
  return;
}