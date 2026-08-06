#include "OutputManager.h"

#include "Constants.h"
#include "TunixErrorManager.h"

OutputManager::OutputManager() 
{ 
  calculateMultipliers();
}

void OutputManager::calculateMultipliers()
{
  voltageMultiplier =     (5.0f / 1023.0f) * ((resistorUp + resistorDown) / resistorDown);
  mainVoltageMultiplier = (5.0f / 1023.0f) * ((resistorMainUp + resistorMainDown) / resistorMainDown);

  currentMultiplier = (5.0f / 1023.0f) / (gain * shuntResistor);

  pwmMultiplier = 255.0f / characteristics::MAX_VOLTAGE;
}

void OutputManager::updateResistors(float rUp, float rDown, float rMainUp, float rMainDown, float shunt, float g)
{
  resistorUp = rUp;
  resistorDown = rDown;
  resistorMainUp = rMainUp;
  resistorMainDown = rMainDown;
  shuntResistor = shunt;
  gain = g;
  
  calculateMultipliers();
}

float OutputManager::readVoltage()
{
  long sum = 0;
  for(int i = 0; i < characteristics::VOLTAGE_SAMPLE_COUNT; i++) sum += analogRead(pins::VOLT_METER);
  
  float average = sum / (float)characteristics::VOLTAGE_SAMPLE_COUNT;

  return average * voltageMultiplier;
}

float OutputManager::readMainVoltage()
{
  long sum = 0;
  for(int i = 0; i < characteristics::VOLTAGE_SAMPLE_COUNT; i++) sum += analogRead(pins::VOLT_METER_MAIN);
  
  float average = sum / (float)characteristics::VOLTAGE_SAMPLE_COUNT;
  float volt = average * mainVoltageMultiplier;

  if(volt <= 11.0f && volt >= 8.0f) errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_LOW);
  if(volt < 8.0f)                   errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_CRITICAL);
  if(volt > 27.0f)                  errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_HIGH);
  
  return volt;
}

float OutputManager::readCurrent()
{
  long sum = 0;
  for(int i = 0; i < characteristics::CURRENT_SAMPLE_COUNT; i++) sum += analogRead(pins::CURRENT_METER);
  
  float average = sum / (float)characteristics::CURRENT_SAMPLE_COUNT;
  return average * currentMultiplier;
}

void OutputManager::toggleRelay()
{
  outputActive = !outputActive;
  digitalWrite(pins::RELAY, outputActive);
}

void OutputManager::setRelay(bool state)
{
  outputActive = state;
  digitalWrite(pins::RELAY, outputActive);
}

void OutputManager::setVoltage(float targetVoltage)
{
  int pwmValue = targetVoltage * pwmMultiplier;

  if (pwmValue > 255) pwmValue = 255;
  if (pwmValue < 0)   pwmValue = 0;
  
  analogWrite(pins::VOLTAGE_CONTROL, pwmValue);
}

void OutputManager::togglePowerBoard(bool state)
{
  digitalWrite(pins::POWER_CONTROL, state);
  digitalWrite(pins::VOLTAGE_CONTROL, state);
}