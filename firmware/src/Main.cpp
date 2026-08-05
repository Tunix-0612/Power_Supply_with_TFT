/*
    Tunix - Tunahan Bayraktar
    Mosfet Power Supply with TL494
    Arduino Nano
    Version 0.7.1 Alpha - Build.26.08.C

    (128 - text width) / 2 -- Small Text Size: 5+1 -- Med. Txt. Size: 10+2

    ---------------------------------------------------------
    CHANGELOG
    ---------------------------------------------------------
    [Added]
    - Variations for textToScreenFull and textToScreenFast functions to support both RAM and FLASH strings.

    [Changed]
    - All static strings in the project have been converted to FLASH strings using the F() macro to optimize RAM usage.
    - Update screen text to indicate "Version difference detected" instead of "Version inconstancy detected" for clarity.
    - Complete overhaul of Display Driver for expansion to display capabilities.
    - Pre-view of Power Supply Mode has been implemented.

    [Removed]
    - Old EEPROM address variables and related code that were deleted.
    - Unused variables has been removed from classes.
    - Display Driver has been compeletely cleaned up from other functions.

    [Fixed]
    - 
*/

#include "Constants.h"

#include "DisplayDriver.h"
#include "PowerSupplyModes.h"
#include "OutputManager.h"
#include "Menu.h"
#include "TempController.h"

#include "InputManager.h"

#include "TunixErrorManager.h"
#include "TunixMemoryManager.h"
#include "TunixSelfTest.h"

TunixErrorManager errorManager;
TunixMemoryManager memory(EEPROM.length());
TunixSelfTest deviceTest;

DisplayDriver display;
PowerSupplyClass powerSupply;
OutputManager output;
MenuClass menu;
TempController temperature;

InputManager inputManager(pins::UP_BUTTON, pins::DOWN_BUTTON, pins::LEFT_BUTTON, pins::RIGHT_BUTTON);

void setup()
{
  digitalWrite(pins::SCREEN_BACKLIGHT, LOW);
  digitalWrite(pins::POWER_CONTROL, LOW);

  pinMode(pins::SCREEN_BACKLIGHT, OUTPUT);
  pinMode(pins::FAN, OUTPUT);
  pinMode(pins::VOLTAGE_CONTROL, OUTPUT);
  pinMode(pins::RELAY, OUTPUT);
  pinMode(pins::BUZZER, OUTPUT);

  pinMode(pins::TEMP_SENSOR, INPUT);
  pinMode(pins::VOLT_METER, INPUT);
  pinMode(pins::CURRENT_METER, INPUT);
  pinMode(pins::VOLT_METER_MAIN, INPUT);
  pinMode(pins::POT, INPUT);

  TCCR1B = (TCCR1B & B11111000) | 0x01;

  inputManager.begin();

  tone(pins::BUZZER, 1500, 50);

  display.initDisplay();
  memory.getBasicMemory();

  display.textToScreenFull(35, 42, ST7735_WHITE, ST7735_BLACK, 2, F("TUNIX"), false);
  display.textToScreenFull(6, 67, ST7735_WHITE, ST7735_BLACK, 2, F("ELECTRONIC"), false);
  delay(350);
  display.textToScreenFull(3, 115, TFT_GRAY, ST7735_BLACK, 1, F("Build.26.08.B"), false);
  tone(pins::BUZZER, 1800, 50);
  delay(1000);
}

void loop()
{
  powerSupply.standbyMode();
  menu.mainMenu();
  errorManager.errorHandler(ErrorCode::PROGRAM_LOOP_FAILURE);
}