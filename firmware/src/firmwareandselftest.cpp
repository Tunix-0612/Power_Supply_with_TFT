#include "Arduino.h"
#include "firmwareandselftest.h"
#include "constants.h"
#include "variables.h"
#include "display.h"
#include "memorymanager.h"
#include "tempcontroller.h"
#include "poweroutputmanager.h"
#include "secondaryfunctions.h"


void firmwareCheck()
{
  EEPROM.get(versionAdress, deviceVersion);
  if(deviceVersion.majorVersion != softwareMajorVersion && deviceVersion.minorVersion != softwareMinorVersion && deviceVersion.patchVersion != softwarePatchVersion)
  {
    tft.fillScreen(ST7735_BLACK);
    tunixBadge();
    textToScreenFull(1, 20, ST7735_WHITE, ST7735_BLACK, 1, ">Version inconstancy detected", true);
    textToScreenFast(1, 1, "\n>Update processing...", true);
    EEPROM.get(currentLimitEepromAdress, settings.currentLimit);
    EEPROM.get(questVoltEepromAdress, settings.questVolt);
    EEPROM.get(batteryModeEepromAdress, settings.batteryMode);
    EEPROM.get(currentProtectionEepromAdress, settings.currentProtectionMode);
    textToScreenFast(1, 1, "\n>Old settings cached.", true);
    delay(200);
    eepromReset(true, false);
    textToScreenFast(1, 1, "\n>Memory Wiped.", true);
    EEPROM.put(settingsAdress, settings);
    delay(200);
    textToScreenFast(1, 1, "\n>Settings saved.", true);
    delay(200);
    textToScreenFast(1, 1, "\n>New tables created.", true);
    delay(400);
    textToScreenFast(1, 1, "\n>Resetting Device...", true);
    wdt_enable(WDTO_2S);
    while(true);
  }
}

void selfTest(bool silent)
{
  bool selfTestFailed = false;
  selfTestActive = true;
  if(!silent)
  {
    tunixBadge();
    textToScreenFull(7, 12, ST7735_WHITE, ST7735_BLACK, 1, "Self Test is ongoing...", false);
  }
  int temperatureStatus = tempControl();
  float voltCalculated = readVoltage();
  float voltMainCalculated = readMainVoltage();
  float currentCalculated = readCurrent();
  if(temperatureStatus == 1 && temperatureStatus == 0) errorHandler(temperatureSensorFailCode);
  if(temperatureStatus >= 85) errorHandler(overheatCode);
  //if(voltCalculated > 1.00) selfTestFailed = true;
  //if(currentCalculated > 0.05) selfTestFailed = true;
  while (selfTestFailed)
  {
    tone(buzzerPin, 1500, 200);
    delay(300);
  }
  firmwareCheck();
  selfTestActive = false;
  return;
}