#include "TunixMemoryManager.h"
#include "Constants.h"
#include "DisplayDriver.h"

TunixMemoryManager::TunixMemoryManager(int EEPROMSize)
{
  _EEPROMSize = EEPROMSize - 1;

  defaultSettings.voltTolerance = 0.1f;
  defaultSettings.currentLimit = 1.0f;
  defaultSettings.currentProtectionMode = true;
  defaultSettings.batteryMode = false;
  defaultSettings.questVolt = 5.0f;

}

void TunixMemoryManager::getBasicMemory()
{
  EEPROM.get(VERSION_ADRESS, eepromVersion);
  EEPROM.get(SETTINGS_ADRESS, settings);
  // --- Add Additional Device Memory Recalls Here
  return;
}

void TunixMemoryManager::saveBasicMemory()
{
  EEPROM.put(SETTINGS_ADRESS, settings);
  // --- Add Additional Device Memory Saves Here
  return;
}

void TunixMemoryManager::factoryReset()
{
  display.clearScreen();
  display.tunixBadge();
  display.textToScreenFull(20, 10, ST7735_WHITE, ST7735_BLACK, 1, "Are you sure?", false);
  display.textToScreenFast(5, 16, "All the settings will be reverted", false);
  display.textToScreenFast(20, 22, "back to original", false);
  
  delay(200);
  while(digitalRead(pins::RIGHT_BUTTON) == HIGH);

  settings = defaultSettings;
  EEPROM.put(SETTINGS_ADRESS, settings);

  eepromVersion = system_version::FIRMWARE;
  EEPROM.put(VERSION_ADRESS, eepromVersion);

  display.clearScreen();
  display.tunixBadge();
  display.textToScreenFast(5, 16, "Device Reset Complete", false);
  display.textToScreenFast(5, 22, "Please restart the device", false);

  delay(500);
  while(true);
}

// --- Validate the firmware versions
ErrorCode TunixMemoryManager::firmwareValidate() 
{
  // If no version difference, return no error
  if (eepromVersion == system_version::FIRMWARE) return ErrorCode::NONE;

  // If the firmware version is greater than the EEPROM version, it means the firmware has been updated.
  if (eepromVersion < system_version::FIRMWARE)
  {
    // If the EEPROM version is less than the minimum supported version, return an error.
    if (eepromVersion < system_version::MIN_SUPPORTED) return ErrorCode::VERSION_NOT_SUPPORTED;

    display.clearScreen();
    display.tunixBadge();
    display.textToScreenFull(1, 20, ST7735_WHITE, ST7735_BLACK, 1, ">Version inconstancy detected", true);
    display.textToScreenFast(1, 1, "\n>Update processing...", true);


    EEPROM.get(voltToleranceEepromAdress, memory.settings.voltTolerance);
    EEPROM.get(currentProtectionEepromAdress, memory.settings.currentProtectionMode);
    EEPROM.get(batteryModeEepromAdress, memory.settings.batteryMode);
    EEPROM.get(questVoltEepromAdress, memory.settings.questVolt);
    EEPROM.get(currentLimitEepromAdress, memory.settings.currentLimit);
    EEPROM.put(SETTINGS_ADRESS, memory.settings);

    delay(2000);

    // Save the new firmware version to EEPROM
    eepromVersion = system_version::FIRMWARE;
    EEPROM.put(VERSION_ADRESS, eepromVersion);

    display.textToScreenFast(1, 1, "\n>Update Complete.", true);
    delay(400);
    display.textToScreenFast(1, 1, "\n>Pls Reset the Device", true);
    
    while (true); // Lock the Device.
  }

  // If device is downgraded return the error/warning
  return ErrorCode::DOWNGRADED_FIRMWARE;
}
