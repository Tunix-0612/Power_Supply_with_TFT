#include "Menu.h"

#include "DisplayDriver.h"
#include "Constants.h"
#include "TempController.h"
#include "PowerSupplyModes.h"
#include "TunixMemoryManager.h"
#include "InputManager.h"

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

void MenuClass::voltageSettingsMenu()
{
  display.clearScreen(ST7735_BLACK);
  display.drawHeader(F("VOLT PROTECTION"));

  bool drawFull = true;
  unsigned long lastTempCheck = millis();

  while(true)
  {
    inputManager.update();

    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);

    if(drawFull)
    {
      display.textToScreenFull(10, 30, ST7735_WHITE, ST7735_BLACK, 1, F("Volt Tolerance:"), false);
      drawFull = false;
    }

    char voltStr[10];
    dtostrf(characteristics::MAX_VOLTAGE, 5, 2, voltStr);
    display.textToScreenFull(20, 50, ST7735_YELLOW, ST7735_BLACK, 2, voltStr, false);
    display.textToScreenFull(85, 50, ST7735_YELLOW, ST7735_BLACK, 2, F("V"), false);

    if(eventUp == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(memory.settings.voltTolerance <= 0.95)
      memory.settings.voltTolerance += 0.05;
    }

    if(eventDown == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(memory.settings.voltTolerance >= 0.10)
      memory.settings.voltTolerance -= 0.05;
    }

    if(eventLeft == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1200, 30);
      memory.saveBasicMemory();
      break;
    }

    if(millis() - lastTempCheck >= 1000)
    {
      lastTempCheck = millis();
      temperature.tempControl();
    }
  }
}

void MenuClass::currentSettingsMenu()
{
  display.clearScreen(ST7735_BLACK);
  display.drawHeader(F("CURR PROTECTION"));

  unsigned long lastTempCheck = millis();

  while(true)
  {
    inputManager.update();

    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);

    display.textToScreenFull(10, 30, ST7735_WHITE, ST7735_BLACK, 1, F("Mode:"), false);

    if (memory.settings.currentProtectionMode == 1) display.textToScreenFull(50, 30, ST7735_GREEN, ST7735_BLACK, 1, F("CC (Limiter) "), false);
    else                                            display.textToScreenFull(50, 30, ST7735_RED, ST7735_BLACK, 1, F("OCP (Cut-Off)"), false);

    display.textToScreenFull(10, 60, ST7735_WHITE, ST7735_BLACK, 1, F("Current Limit:"), false);
    
    char currStr[10];
    dtostrf(memory.settings.currentLimit, 5, 2, currStr);
    display.textToScreenFull(20, 80, ST7735_YELLOW, ST7735_BLACK, 2, currStr, false);
    display.textToScreenFull(85, 80, ST7735_YELLOW, ST7735_BLACK, 2, F("A"), false);

    if(eventUp == BTN_EVENT_CLICK || eventDown == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      memory.settings.currentProtectionMode = !memory.settings.currentProtectionMode;
    }

    if(eventLeft == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1200, 30);
      memory.saveBasicMemory();
      break;
    }

    if(millis() - lastTempCheck >= 1000)
    {
      lastTempCheck = millis();
      temperature.tempControl();
    }
  }
}

void MenuClass::backlightSettingsMenu()
{
  display.clearScreen(ST7735_BLACK);
  display.drawHeader(F("BACKLIGHT"));

  unsigned long lastTempCheck = millis();

  while(true)
  {
    inputManager.update();

    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);

    display.textToScreenFull(10, 35, ST7735_WHITE, ST7735_BLACK, 1, F("Brightness:"), false);

    char levelStr[8];
    snprintf(levelStr, sizeof(levelStr), "%%%d  ", memory.settings.backlightLevel);
    display.textToScreenFull(35, 60, ST7735_CYAN, ST7735_BLACK, 2, levelStr, false);

    uint8_t barWidth = map(memory.settings.backlightLevel, 0, 100, 0, 108);
    display.getRawDisplay().drawRect(10, 90, 108, 12, ST7735_WHITE);
    display.getRawDisplay().fillRect(11, 91, barWidth, 10, ST7735_GREEN);
    display.getRawDisplay().fillRect(11 + barWidth, 91, 106 - barWidth, 10, ST7735_BLACK);

    if(eventUp == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(memory.settings.backlightLevel <= 90) 
      {
        memory.settings.backlightLevel += 10;
        analogWrite(pins::SCREEN_BACKLIGHT, map(memory.settings.backlightLevel, 0, 100, 0, 255));
      }
    }

    if(eventDown == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(memory.settings.backlightLevel >= 10) 
      {
        memory.settings.backlightLevel -= 10;
        analogWrite(pins::SCREEN_BACKLIGHT, map(memory.settings.backlightLevel, 0, 100, 0, 255));
      }
    }

    if(eventLeft == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1200, 30);
      memory.saveBasicMemory();
      break;
    }

    if(millis() - lastTempCheck >= 1000)
    {
      lastTempCheck = millis();
      temperature.tempControl();
    }
  }
}

void MenuClass::settingsMenu()
{
  display.clearScreen();
  uint8_t settingsCursor = 1;
  uint8_t lastCursor = 0;
  bool drawFull = true;

  unsigned long lastDeviceCheck = millis();

  while(true)
  {
    inputManager.update();

    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);
    ButtonEvent eventRight = inputManager.getEvent(BTN_RIGHT);

    if(drawFull)
    {
      display.clearScreen(ST7735_BLACK);
      display.drawHeader(F("SETTINGS"));

      display.textToScreenFull(20, 30, ST7735_WHITE, ST7735_BLACK, 1, F("Volt Protection"), false);
      display.textToScreenFull(20, 50, ST7735_WHITE, ST7735_BLACK, 1, F("Curr Protection"), false);
      display.textToScreenFull(20, 70, ST7735_WHITE, ST7735_BLACK, 1, F("Backlight"), false);

      uint8_t initY = 30 + ((settingsCursor - 1) * 20);
      display.drawCircleShape(8, initY + 3, 3, ST7735_WHITE, true);
      
      lastCursor = settingsCursor;
      drawFull = false;
    }

    if(settingsCursor != lastCursor)
    {
      if(lastCursor > 0) 
      {
        uint8_t oldY = 30 + ((lastCursor - 1) * 20);
        display.drawCircleShape(8, oldY + 3, 3, ST7735_BLACK, true);
      }

      uint8_t newY = 30 + ((settingsCursor - 1) * 20);
      display.drawCircleShape(8, newY + 3, 3, ST7735_WHITE, true);
      
      lastCursor = settingsCursor;
    }

    if(eventUp == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(settingsCursor > 1) settingsCursor--;
    }

    if(eventDown == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      if(settingsCursor < 3) settingsCursor++;
    }

    if(eventRight == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1800, 50);
      
      switch(settingsCursor)
      {
        case 1: 
          voltageSettingsMenu();
          break;
        case 2: 
          currentSettingsMenu();
          break;
        case 3: 
          backlightSettingsMenu();
          break;
      }
      drawFull = true; 
    }

    if(eventLeft == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1200, 30);
      break;
    }

    if(millis() - lastDeviceCheck >= 1000)
    {
      lastDeviceCheck = millis();
      temperature.tempControl();
    }
  }
}

void MenuClass::menuSelectionCursor(uint8_t currentCursor, uint8_t previousCursor)
{
  if (previousCursor >= 1 && previousCursor <= 4) 
  {
    uint8_t oldY = 20 + (previousCursor - 1) * 17;
    display.drawBox(2, oldY, 123, 13, ST7735_BLACK, false);
  }

  uint8_t newY = 20 + (currentCursor - 1) * 17;
  display.drawBox(2, newY, 123, 13, ST7735_WHITE, false);
}

void MenuClass::mainMenu()
{
  display.clearScreen();
  menuCursor = 1;
  uint8_t lastCursor = 0;
  bool drawFullMenu = true;

  unsigned long lastTempCheck = millis();
  unsigned long lastActivityTime = millis();

  while(true)
  {
    inputManager.update();

    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    ButtonEvent eventRight = inputManager.getEvent(BTN_RIGHT);

    if(drawFullMenu)
    {
      display.clearScreen();
      tunixBadge();
      display.getRawDisplay().drawFastHLine(12, 34, 105, TFT_GRAY);
      display.getRawDisplay().drawFastHLine(12, 51, 105, TFT_GRAY);
      display.getRawDisplay().drawFastHLine(12, 68, 105, TFT_GRAY);
      display.getRawDisplay().drawFastHLine(12, 85, 105, TFT_GRAY);
      display.getRawDisplay().drawFastHLine(12, 102, 105, TFT_GRAY);

      display.textToScreenFull(34, 23, ST7735_WHITE, ST7735_BLACK, 1 , F("Basic Mode"), false);
      display.textToScreenFast(25, 40, F("Advanced Mode"), false);
      display.textToScreenFast(40, 57, F("Settings"), false);
      display.textToScreenFast(31, 74, F("Information"), false);
      
      lastCursor = 0;
      menuSelectionCursor(menuCursor, lastCursor);
      lastCursor = menuCursor;

      drawFullMenu = false;
      lastActivityTime = millis();
    }

    if(eventUp == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      
      if(menuCursor > 1) {
        menuCursor--;
        menuSelectionCursor(menuCursor, lastCursor);
        lastCursor = menuCursor;
      }
      lastActivityTime = millis();
    }

    if(eventDown == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);

      if(menuCursor < 4) {
        menuCursor++;
        menuSelectionCursor(menuCursor, lastCursor);
        lastCursor = menuCursor;
      }
      lastActivityTime = millis();
    }

    if(eventRight == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1800, 50);
      lastActivityTime = millis();

      switch(menuCursor)
      {
        case 1: powerSupply.basicMod(); break;
        case 2: powerSupply.advancedMod(); break;
        case 3: settingsMenu(); break;
        case 4: break;
      }
      drawFullMenu = true;
    }

    if(millis() - lastTempCheck >= 1000) 
    {
      lastTempCheck = millis();
      temperature.tempControl();
    }

    if(millis() - lastActivityTime >= 60000) 
    {
      powerSupply.standbyMode();
      drawFullMenu = true;
    }
  }
}