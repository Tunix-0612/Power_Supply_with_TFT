/*
    BYTunix - Tunahan Bayraktar
    Mosfet Power Supply
    Revived at 25/03/2026 - x
    Arduino Nano
    Version 1.0.0 Full Release - Build.26.06.A
    Update Name --- Full Release --- Major Update

    (128 - text width) / 2 -- Small Text Size: 5+1 -- Med. Txt. Size: 10+2

    

    ---------------------------------------------------------
    CHANGELOG
    ---------------------------------------------------------
    [Added]
    - Watchdog timer has been implemented
    - Firmware checker added with multiple functions with fail-safes
    - An Error Handler function has been added for all the problems within hardware and software
    - New struct type settings and version datas to hold the information effecient and organized
    - Default settings are created with in setting struct
    - Input voltage measurements has been added
      * Device now reacts according to the input voltage levels

    [Changed]
    - Screen accsess methode and functions completely overhauled for better RAM and ROM usage
      * Screen write methode now relies on two new functions instead of direct code access
    - Voltage Limit dropped to 24V from 30V due to board limitations
    - Advanced Mode Screen Lines has been changed to Power Supply Mode Lines for common usage
    - For Gray elements on the screen instead of calculating color for every command, now gray is pre-defined
    - Memory management has been compelety overhauled and optimized
    - Ram usage has been reduced significantly
    - Sleep Mode has been changed as Standby Mode and some of it functions has been tweaked
    - Selftest function has been altered and tweaked
    - Selftest is now cannot be called by user but automatically called every startup

    [Removed]
    - Basic Mode Screen Lines function has been removed as its temporarily disabled
    - Basic Mode Screen Manager function has ben disabled temporarly as it will be implemented on the next updatess
    - Some unused or re-defined variables has been removed
    - Temperature overheat an fail functions has been removed as they moved to Error Handler

    [Fixed]
    - Start-up sequance has been tweaked for better handling
    - Flash Memory has been optimized
    - Fan speed adjusted to be faster at lower tempuratures as tempurature sensor will be mounted at the heatsink joint between diode and mosfet
    - Some math operations has been simplifed

    ---------------------------------------------------------
    Aim of the next updates
    ---------------------------------------------------------
    - Variable optimization and clean-up                [%90]
    - Continuation of splitting program into pieces     [%99]
    - Expansion from 3 buttons to 4                     [%0]
    - PWM and control logic overhaul for TL494 Chip     [%60]
    - Voltage calibration ability                       [%0]
    - Better calls between functions                    [%70]
    - Fix recursive function problem                    [%50]
    - Re-order program flow                             [%60]
    - Flash Memory Optimizations                        [%50]
    - Screen management optimizations                   [%80]
    - Re-work tempurature operations                    [%60]
    - Re-work UI                                        [%0]
    ---------------------------------------------------------
*/

#include <avr/wdt.h>

#include "constants.h"
#include "variables.h"
#include "display.h"
#include "tempcontroller.h"
#include "poweroutputmanager.h"
#include "secondaryfunctions.h"
#include "mainmodes.h"
#include "menu.h"
#include "memorymanager.h"
#include "firmwareandselftest.h"


void setup()
{
  wdt_disable();
  digitalWrite(screenBacklightPin, LOW);
  digitalWrite(powerControlPin, LOW);
  pinMode(screenBacklightPin, OUTPUT);
  pinMode(topButtonPin, INPUT_PULLUP);
  pinMode(bottomButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  pinMode(voltageControlPin, OUTPUT);
  pinMode(tempSensorPin, INPUT);
  pinMode(voltMeterPin, INPUT);
  pinMode(currentMeterPin, INPUT);
  pinMode(voltMeterMainPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(potPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  TCCR1B = TCCR1B & B11111000 | 0x01;
  tone(buzzerPin, 1500, 50);
  initDisplay();
  selfTest(true);
  initMemory();
  textToScreenFull(35, 42, ST7735_WHITE, ST7735_BLACK, 2, "TUNIX", false);
  textToScreenFull(6, 67, ST7735_WHITE, ST7735_BLACK, 2, "ELECTRONIC", false);
  delay(350);
  textToScreenFull(3, 115, TFT_GRAY, ST7735_BLACK, 1, "Build.26.06.A", false);
  tone(buzzerPin, 1800, 50);
  delay(1000);
}

void loop()
{
  standbyMode();
  mainMenu();
  errorHandler(programLoopFailureCode);
}