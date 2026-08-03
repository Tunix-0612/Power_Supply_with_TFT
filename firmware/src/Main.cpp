/*
    Tunix - Tunahan Bayraktar
    Mosfet Power Supply with TL494
    Arduino Nano
    Version 0.7.0 Alpha - Build.26.08.B

    (128 - text width) / 2 -- Small Text Size: 5+1 -- Med. Txt. Size: 10+2

    ---------------------------------------------------------
    CHANGELOG
    ---------------------------------------------------------
    [Added]
    - Tunix Core Modules V3 has been integrated into the project.
    - Tunix Input Manager has been integrated into the project.

    [Changed]
    - All functions has been re-structured and re-written to be more efficient and readable.
    - Complete overhaul towards C++11 standards and practices.

    [Removed]
    - Unrequired definitions has been removed.
    - Old #define versioning has been removed and replaced with a struct.
    - Constants.h has been re-structured and cleaned up with C++ standards.
    - Old firmware files have been removed and replaced with Tunix Core Modules V3.

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

InputManager inputManager(pins::TOP_BUTTON, pins::BOTTOM_BUTTON, pins::LEFT_BUTTON, pins::RIGHT_BUTTON);

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

  display.textToScreenFull(35, 42, ST7735_WHITE, ST7735_BLACK, 2, "TUNIX", false);
  display.textToScreenFull(6, 67, ST7735_WHITE, ST7735_BLACK, 2, "ELECTRONIC", false);
  delay(350);
  display.textToScreenFull(3, 115, TFT_GRAY, ST7735_BLACK, 1, "Build.26.08.B", false);
  tone(pins::BUZZER, 1800, 50);
  delay(1000);
}

void loop()
{
  powerSupply.standbyMode();
  menu.mainMenu();
  errorManager.errorHandler(ErrorCode::PROGRAM_LOOP_FAILURE);
}