#include "menu.h"
#include "constants.h"
#include "variables.h"
#include "display.h"
#include "tempcontroller.h"
#include "secondaryfunctions.h"
#include "mainmodes.h"
#include "memorymanager.h"

static byte menuCursor = 1, sleepTime;
static int returnTime;

void menuSelectionCursor()
{
  short cursorXPosition;
  for(short g = 20; g < 106; g = g + 17) tft.drawRect(2, g, 123, 13, ST7735_BLACK);
  if(menuCursor == 1) cursorXPosition = 20;
  if(menuCursor == 2) cursorXPosition = 37;
  if(menuCursor == 3) cursorXPosition = 54;
  if(menuCursor == 4) cursorXPosition = 71;
  if(menuCursor == 5) cursorXPosition = 88;
  if(menuCursor == 6) cursorXPosition = 105;
  tft.drawRect(2, cursorXPosition, 123, 13, tft.color565(153, 255, 255));
  return;
}

void infoMenu()
{
  short textPos, textCount = 1;
  tft.fillScreen(ST7735_BLACK);
  textToScreenFull(16, 5, TFT_GRAY, ST7735_BLACK, 1, "Tunix Electronics", false);
  lineToScreen(2, 14, 126, 14, TFT_GRAY);
  for (textCount = 17; textPos < 128; textPos = textPos + 10)
  {
    switch(textCount)
    {
      case 1: textToScreenFull(3, textPos, TFT_GRAY, ST7735_BLACK, 1, "MCU", false); break;
      case 2: textToScreenFast(3, textPos, "MHZ", false); break;
      case 3: textToScreenFast(3, textPos, "GPU", false); break;
      case 4: textToScreenFast(3, textPos, "GFX.API", false); break;
      case 5: textToScreenFast(3, textPos, "ROM", false); break;
      case 6: textToScreenFast(3, textPos, "RAM", false); break;
      case 7: textToScreenFast(3, textPos, "OS", false); break;
      case 8: textToScreenFast(3, textPos, "OS VER", false); break;
      case 9: textToScreenFast(3, textPos, "BUILD", false); break;
      case 10: textToScreenFast(3, textPos, "VOLT LIM", false); break;
      case 11: textToScreenFast(3, textPos, "SCREEN", false); break;
    }
    textCount++;
  }
  textCount = 1;
  tft.setTextColor(ST7735_WHITE);
  for (textPos = 17; textPos < 128; textPos = textPos + 10) 
  {
    tft.setCursor(60, textPos);
    switch(textCount)
    {
      case 1: textToScreenFull(60, textPos, ST7735_WHITE, ST7735_BLACK, 1, "Atmega328P", false); break;
      case 2: textToScreenFast(3, textPos, "16MHZ", false); break;
      case 3: textToScreenFast(3, textPos, "ST7735", false); break;
      case 4: textToScreenFast(3, textPos, "ADAFRUIT", false); break;
      case 5: textToScreenFast(3, textPos, "NaN/30720", false); break;
      case 6: textToScreenFast(3, textPos, "NaN/2048", false); break;
      case 7: textToScreenFast(3, textPos, "Tunix PS.UI", false); break;
      case 8: textToScreenFast(3, textPos, "V0.6.0", false); break;
      case 9: textToScreenFast(3, textPos, "26.08.A", false); break;
      case 10: textToScreenFast(3, textPos, "0-24V", false); break;
      case 11: textToScreenFast(3, textPos, "128x128 SPI", false); break;
    }
    textCount++;
  }
  textCount = 1;
  while(digitalRead(leftButtonPin) == HIGH)
  {
    delay(2);
    sleepTime++;
    if(sleepTime >= 30000) standbyMode();
  }
  while(digitalRead(topButtonPin) == LOW);
  tone(buzzerPin, 1800, 50);
  return;
}

void settingsMenu()
{
  tft.fillScreen(ST7735_BLACK);
  menuCursor = 1;
  tunixBadge();
  menuGridLines();
  settingsMenuText();
  settingsMenuTextData();
  menuSelectionCursor();
  while(true)
  {
    if(digitalRead(topButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      while(digitalRead(topButtonPin) == LOW);
      menuCursor--;
      menuCursor = constrain(menuCursor, 1, 6);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(bottomButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      while(digitalRead(bottomButtonPin) == LOW);
      menuCursor++;
      menuCursor = constrain(menuCursor, 1, 6);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(rightButtonPin) == LOW)
    {
      tone(buzzerPin, 1800, 60);
      menuSelectionCursor();
      while(digitalRead(rightButtonPin) == LOW);
      
      if(menuCursor == 2)
      {
        short buttonHoldTime, flexibleDelay;
        while (digitalRead(leftButtonPin) == HIGH)
        {
          buttonHoldTime = 0;
          flexibleDelay = 200;
          returnTime++;
          delay(2);
          if(returnTime >= 7500) break;
          while (digitalRead(topButtonPin) == LOW)
          {
            tone(buzzerPin, 1200, 20);
            buttonHoldTime++;
            settings.currentLimit =+ 0.1;
            settings.currentLimit = constrain(settings.currentLimit, 0.1, 20.0);
            if(buttonHoldTime > 10) flexibleDelay = 100;
            if(buttonHoldTime > 25) flexibleDelay = 75;
            returnTime = 0;
            settingsMenuTextData();
            delay(flexibleDelay);
          }
          while (digitalRead(bottomButtonPin) == LOW)
          {
            tone(buzzerPin, 1200, 20);
            buttonHoldTime++;
            settings.currentLimit =- 0.1;
            settings.currentLimit = constrain(settings.currentLimit, 0.1, 20.0);
            if(buttonHoldTime > 10) flexibleDelay = 100;
            if(buttonHoldTime > 25) flexibleDelay = 75;
            returnTime = 0;
            settingsMenuTextData();
            delay(flexibleDelay);
          }
        }
        tone(buzzerPin, 1800, 60);
        returnTime = 0;
        while(digitalRead(leftButtonPin) == LOW);
      }
      if(menuCursor == 3)
      {
        while(digitalRead(rightButtonPin) == LOW);
        settings.currentProtectionMode = !settings.currentProtectionMode;
        settingsMenuTextData();
      }
      if(menuCursor == 4)
      {
        while(digitalRead(rightButtonPin) == LOW);
        settings.batteryMode = !settings.batteryMode;
        settingsMenuTextData();
      }
      if(menuCursor == 5)
      {
        while(digitalRead(rightButtonPin) == LOW);
        eepromReset(false, true);
      }
      if(menuCursor == 6)
      {
        while(digitalRead(rightButtonPin) == LOW);
        menuCursor = 1;
        return;
      }
    }
    sleepTime++;
    if(sleepTime >= 30000) sleepTime = standbyMode();
    delay(2);
  }
  sleepTime = 0;
  return;
}


void mainMenu()
{
  while(digitalRead(leftButtonPin) == LOW);
  tft.fillScreen(ST7735_BLACK);
  menuCursor = 1;
  bool drawMenu = true;
  int tempControlTime;
  while(true)
  {
    if(drawMenu == true)
    {
      tft.fillScreen(ST7735_BLACK);
      tunixBadge();
      menuGridLines();
      mainMenuText();
      menuSelectionCursor();
      drawMenu = false;
    }
    if(digitalRead(topButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      while(digitalRead(topButtonPin) == LOW);
      menuCursor--;
      menuCursor = constrain(menuCursor, 1, 4);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(bottomButtonPin) == LOW)
    {
      tone(buzzerPin, 1500, 20);
      while(digitalRead(bottomButtonPin) == LOW);
      menuCursor++;
      menuCursor = constrain(menuCursor, 1, 4);
      sleepTime = 0;
      menuSelectionCursor();
    }
    if(digitalRead(rightButtonPin) == LOW)
    {
      tone(buzzerPin, 1800, 50);
      sleepTime = 0;
      while(digitalRead(rightButtonPin) == LOW);
      switch(menuCursor)
      {
        case 1: basicMod(); break;
        case 2: advancedMod(); break;
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
      tempControl();
      tempControlTime = 0;
    }
    if(sleepTime >= 30000) standbyMode();
  }
  return;
}