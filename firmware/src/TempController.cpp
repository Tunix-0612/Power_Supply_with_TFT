#include "TempController.h"

#include "Constants.h"
#include "TunixErrorManager.h"
#include "OutputManager.h"

TempController::TempController() { }

int TempController::readTemperature()
{
  int temp;
  temp = analogRead(pins::TEMP_SENSOR);
  temp = temp * 0.48828125;
  return temp;
}

uint8_t TempController::getPowerAggression(float currentPower)
{
    if (currentPower >= 100.0f) 
    {
        isHighPowerActive = true; 
        return 3;
    } 

    else if (currentPower >= 50.0f) 
    {
        if (!isHighPowerActive) 
        {
            isHighPowerActive = true;
            highPowerStartTime = millis();
        }

        unsigned long duration = millis() - highPowerStartTime;

        if (duration >= 90000) return 2;
        if (duration >= 30000)  return 1;
        
        return 0;
    } 
    else 
    {
        isHighPowerActive = false;
        return 0;
    }
}

uint8_t TempController::getTempAggression(int currentTemp)
{
  if (currentTemp >= 65) return 4;
  if (currentTemp >= 55) return 3;
  if (currentTemp >= 45) return 2;
  if (currentTemp >= 35) return 1;
    
  return 0;
}

void TempController::applyAggression(uint8_t level)
{
    if (currentAggression == level) return;
    
    currentAggression = level;
    
    switch (currentAggression) 
    {
        case 1: currentFanPWM = 80;  break;
        case 2: currentFanPWM = 125; break;
        case 3: currentFanPWM = 190; break;
        case 4: currentFanPWM = 255; break;
        default: currentFanPWM = 0;  break;
    }

    analogWrite(pins::FAN, currentFanPWM);
}

void TempController::tempControl()
{
    float currentTemp = readTemperature();
    float currentPower = output.readVoltage() * output.readCurrent();

    uint8_t powerLevel = getPowerAggression(currentPower);
    uint8_t tempLevel  = getTempAggression(currentTemp);

    uint8_t finalLevel = max(powerLevel, tempLevel);

    applyAggression(finalLevel);

    if (currentTemp >= 140) 
    {
        tone(pins::BUZZER, 2000, 500);
        errorManager.errorHandler(ErrorCode::TEMP_SENSOR_FAIL);
    }
    
    if (currentTemp >= 80) 
    {
        tone(pins::BUZZER, 2000, 500);
        errorManager.errorHandler(ErrorCode::OVERHEAT);
    }
}