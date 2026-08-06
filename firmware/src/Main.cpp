/*
    Tunix - Tunahan Bayraktar
    Mosfet Power Supply with TL494
    Arduino Nano
    Version 0.7.4 Alpha - Build.26.08.F

    (128 - text width) / 2 -- Small Text Size: 5+1 -- Med. Txt. Size: 10+2

    ---------------------------------------------------------
    CHANGELOG
    ---------------------------------------------------------
    [Added]
    - Display Driver now supports isolated screen toggling.
    - Output Manager now supports isolated Power Board toggling.
    - Now waking up from standby mode triggers another Self Check.

    [Changed]
    - EEPROM Library definition has been moved to .cpp to isolation.
    - Minor definition order changes.
    - Name for the PowerSupplyModes has been changed to PowerSupplyManager.
    - Standby Function has been improved and re-written with new standards.
    - Standby Function now acts as when called by the Menu Timeout event instead of at the boot.
    
    [Removed]
    - Removed unrequired definitions.
    - Faulty v0.7.1 specific update has been removed.

    [Fixed]
    - [!] Device not checking firmware version has been fixed.
    - [!] Device not using self test functions has been fixed.
    - Minor self test bug-fix due to new Output Manager capabilities.

    [Notice]
      New UI features are experimental as they are not expected to work properly yet or work at all.
      The "Alpha" tag usually means the version is not TESTED at all nor ready-to-use.
      The "Beta" versions will be tested on board and diagnosed for any problems.
      For now visual bugs and unintended behavior is expected.
*/

#include "Constants.h"

#include "DisplayDriver.h"
#include "PowerSupplyManager.h"
#include "OutputManager.h"
#include "Menu.h"
#include "TempController.h"

#include "InputManager.h"

#include "TunixErrorManager.h"
#include "TunixMemoryManager.h"
#include "TunixSelfTest.h"

TunixErrorManager errorManager;
TunixMemoryManager memory(1024);
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

  deviceTest.selfTest();

  memory.firmwareValidate();

  display.textToScreenFull(35, 42, ST7735_WHITE, ST7735_BLACK, 2, F("TUNIX"), false);
  display.textToScreenFull(6, 67, ST7735_WHITE, ST7735_BLACK, 2, F("ELECTRONIC"), false);
  delay(350);
  display.textToScreenFull(3, 115, TFT_GRAY, ST7735_BLACK, 1, F("Build.26.08.B"), false);
  tone(pins::BUZZER, 1800, 50);
  delay(1000);
}

void loop()
{
  menu.mainMenu();

  errorManager.errorHandler(ErrorCode::PROGRAM_LOOP_FAILURE);
}