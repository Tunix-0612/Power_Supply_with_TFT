#include "memorymanager.h"
#include "constants.h"
#include "variables.h"

void initMemory()
{
  EEPROM.get(versionAdress, deviceVersion);
  EEPROM.get(settingsAdress, settings);
  return;
}