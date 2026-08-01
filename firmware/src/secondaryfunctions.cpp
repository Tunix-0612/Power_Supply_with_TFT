#include "Arduino.h"
#include "secondaryfunctions.h"
#include "constants.h"
#include "variables.h"
#include "display.h"
#include "poweroutputmanager.h"
#include "tempcontroller.h"
#include "memorymanager.h"


void errorHandler(int errorCode)
{
  tft.fillScreen(ST7735_BLUE);
  lineToScreen(0, 10, 128, 10, TFT_GRAY);
  textToScreenFull(34, 8, ST7735_WHITE, ST7735_BLUE, 2, "FAULT", false);
  char errorCodeStr[5];
  itoa(errorCode, errorCodeStr, 10);
  textToScreenFull(4, 20, ST7735_WHITE, ST7735_BLUE, 1, errorCodeStr, true);
  if(errorCode == firmwareVersionErrorCode)
  {
    textToScreenFast(1, 1, " - Firmware Version Error", true);
    textToScreenFast(1, 1, "\n Please reset device, ok to reset", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
    eepromReset(false, true);
    while(true);
  }
  if(errorCode == versionNotSupportedCode)
  {
    textToScreenFast(1, 1, " - Version not supported", true);
    textToScreenFast(1, 1, "\n Please update to a valid version", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
  }
  if(errorCode == unknownFirmwareErrorCode)
  {
    textToScreenFast(1, 1, " - Unknown Firmware Error", true);
    textToScreenFast(1, 1, "\n Please reset device, ok to factory reset", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
    eepromReset(false, true);
    while(true);
  }
  if(errorCode == downGradedFirmwareErrorCode)
  {
    textToScreenFast(1, 1, " - Version Downgraded", true);
    textToScreenFast(1, 1, "\n Device may not function properly", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
  }
  if(errorCode == programLoopFailureCode)
  {
    textToScreenFast(1, 1, " - Program error", true);
    textToScreenFast(1, 1, "\n Device will be reset", true);
    wdt_enable(WDTO_2S);
    while(true);
  }
  if(errorCode == overheatCode)
  {
    digitalWrite(fanPin, HIGH);
    textToScreenFast(1, 1, " - Overheat", true);
    textToScreenFast(1, 1, "\n Wait while device cools down", true);
    while ((temperature = tempControl()) > 70) delay(1000);
  }
  if(errorCode == temperatureSensorFailCode)
  {
    tempSensorFailed = true;
    digitalWrite(fanPin, HIGH);
    textToScreenFast(1, 1, " - Tempurature Sensor Fail", true);
    textToScreenFast(1, 1, "\n Fan will be on maximum speed", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
  }
  if(errorCode == voltageFluctuateErrorCode);
  if(errorCode == voltageCalibrationErrorCode);
  if(errorCode == currentCalculationFailErrorCode);
  if(errorCode == mainVoltageLowErrorCode)
  {
    digitalWrite(relayPin, LOW);
    relayPosition = false;
    textToScreenFast(1, 1, " - Low Input Voltage", true);
    while (digitalRead(rightButtonPin) == HIGH);
    delay(200);
    lowVoltageAcknowledged = true;
  }
  if(errorCode == mainVoltageCriticalErrorCode)
  {
    digitalWrite(relayPin, LOW);
    relayPosition = false;
    textToScreenFast(1, 1, " - Critically Low Input Voltage", true);
    textToScreenFast(1, 1, "\n PLEASE DO NOT USE 12V OUT", true);
    while (true);
  }
  if(errorCode == mainVoltageHighErrorCode)
  {
    digitalWrite(powerControlPin, LOW);
    digitalWrite(relayPin, LOW);
    relayPosition = false;
    textToScreenFast(1, 1, " - Too High Input Voltage", true);
    while (true);
    delay(200);
  }
}

void eepromReset(bool silent, bool setDefaults)
{
  if(!silent)
  {
    tft.fillScreen(ST7735_BLACK);
    tunixBadge();
    textToScreenFull(20, 10, ST7735_WHITE, ST7735_BLACK, 1, "Are you sure?", false);
    textToScreenFast(5, 16, "All the settings will be reverted", false);
    textToScreenFast(20, 22, "back to original", false);
  }
  for (int counter = 0; counter < 255; counter++) EEPROM.write(counter, 255);
  
  deviceVersion.majorVersion = softwareMajorVersion;
  deviceVersion.minorVersion = softwareMinorVersion;
  deviceVersion.patchVersion = softwarePatchVersion;
  EEPROM.put(versionAdress, deviceVersion);
  
  if(setDefaults)
  {
    EEPROM.put(settingsAdress, defaultSettings);
    EEPROM.get(settingsAdress, settings);
  }
  delay(200);
}