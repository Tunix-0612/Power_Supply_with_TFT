#include "TunixErrorManager.h"
#include "TunixMemoryManager.h"
#include "DisplayDriver.h"

TunixErrorManager::TunixErrorManager() { }

// --- Add Additional Error Handler Functions Here

void TunixErrorManager::errorHandler(ErrorCode code) // --- Standart Error Handlers
{
  byte numericCode = static_cast<byte>(code);
  char errorCodeStr[4];
  itoa(numericCode, errorCodeStr, 10);

  switch(code) 
  {
    case ErrorCode::FIRMWARE_VERSION_ERROR:

      display.textToScreenFast(1, 1, F(" - Firmware Version Error"), true);
      display.textToScreenFast(1, 1, F("\n Please reset device, ok to reset"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      memory.factoryReset(); // Recommended
      break;

    case ErrorCode::VERSION_NOT_SUPPORTED:
      
      display.textToScreenFast(1, 1, F(" - Version not supported"), true);
      display.textToScreenFast(1, 1, F("\n Please reset device, ok to reset"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      memory.factoryReset(); // Recommended
      break;

    case ErrorCode::DOWNGRADED_FIRMWARE:

      display.textToScreenFast(1, 1, F(" - Version Downgraded"), true);
      display.textToScreenFast(1, 1, F("\n Device may not function properly, reset recommended."), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      break;

    case ErrorCode::PROGRAM_LOOP_FAILURE:

      display.textToScreenFast(1, 1, F(" - Program error"), true);
      display.textToScreenFast(1, 1, F("\n Reset the device"), true);
      while(true); // Force-Reset Recommended
      break;

    case ErrorCode::RAM_FAILURE:

      display.textToScreenFast(1, 1, F(" - RAM Error"), true);
      display.textToScreenFast(1, 1, F("\n Faulty Device"), true);
      while(true); // Faulty Micro Controller
      break;

    case ErrorCode::OVERHEAT:
    
      display.textToScreenFast(1, 1, F(" - Overheat"), true);
      display.textToScreenFast(1, 1, F("\n Wait while device cools down"), true);
      
      // ADD TEMP CONTROL HERE
      break;

    case ErrorCode::TEMP_SENSOR_FAIL:
      display.textToScreenFast(1, 1, F(" - Temperature Sensor Fail"), true);
      display.textToScreenFast(1, 1, F("\n Fan will be on maximum speed"), true);
      
      // ADD TEMP CONTROL HERE
      delay(200);
      break;

    case ErrorCode::VOLTAGE_FLUCTUATE:
      display.textToScreenFast(1, 1, F(" - Voltage Fluctuation"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    case ErrorCode::VOLTAGE_CALIBRATION:
      display.textToScreenFast(1, 1, F(" - Voltage Calibration Error"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    case ErrorCode::CURRENT_CALC_FAIL:
      display.textToScreenFast(1, 1, F(" - Current Calculation Error"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    case ErrorCode::MAIN_VOLTAGE_LOW:
      display.textToScreenFast(1, 1, F(" - Low Input Voltage"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    case ErrorCode::MAIN_VOLTAGE_CRITICAL:
      display.textToScreenFast(1, 1, F(" - Critical Input Voltage"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    case ErrorCode::MAIN_VOLTAGE_HIGH:
      display.textToScreenFast(1, 1, F(" - High Input Voltage"), true);
      display.textToScreenFast(1, 1, F("\n Check the input voltage"), true);
      while (digitalRead(pins::RIGHT_BUTTON) == HIGH);
      delay(200);
      break;

    default:

      display.textToScreenFast(1, 1, F(" - Unknown Error"), true);
      display.textToScreenFast(1, 1, F("\n Reset the device"), true);
      while(true); // Unknown Error
      break;
  }

  return;
}
