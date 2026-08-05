#include "OutputManager.h"
#include "Constants.h"
#include "DisplayDriver.h"
#include "TunixErrorManager.h"

OutputManager::OutputManager() { }

float OutputManager::readVoltage()
{
  long sum = 0;

  for(int i = 0; i < characteristics::VOLTAGE_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(pins::VOLT_METER);
    delay(1);
  }

  float average = sum / (float)characteristics::VOLTAGE_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float volt = pinVolt * ((resistorUp + resistorDown) / resistorDown);

  return volt;
}

float OutputManager::readMainVoltage()
{
  long sum = 0;

  for(int i = 0; i < characteristics::VOLTAGE_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(pins::VOLT_METER_MAIN);
    delay(1);
  }

  float average = sum / (float)characteristics::VOLTAGE_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float volt = pinVolt * ((resistorMainUp + resistorMainDown) / resistorMainDown);
  
  if(volt <= 11 && volt >= 8 && volt == false) errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_LOW);
  if(volt < 8) errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_CRITICAL);
  if(volt > 27) errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_HIGH);
  
  return volt;
}

float OutputManager::readCurrent()
{
  long sum = 0;

  for(int i = 0; i < characteristics::CURRENT_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(pins::CURRENT_METER);
    delay(1);
  }

  float average = sum / (float)characteristics::CURRENT_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float resistorVolt = pinVolt / gain;
  float current = resistorVolt / shuntResistor;

  return current;
}

void OutputManager::relayChange()
{
  tone(pins::BUZZER, 1800, 50);
  while (digitalRead(pins::UP_BUTTON) == LOW);
  delay(100);
  relayPosition = !relayPosition;
  digitalWrite(pins::RELAY, relayPosition);
  firstScreenWrite = 1;
  display.drawBox(85, 29, 40, 14, ST7735_BLACK);
  if(relayPosition == true)   display.textToScreenFull(95, 29, ST7735_WHITE, ST7735_BLACK, 2, F("On"), false);
  if(relayPosition == false)  display.textToScreenFull(89, 29, ST7735_WHITE, ST7735_BLACK, 2, F("Off"), false);

  return;
}

void OutputManager::relayOff()
{
  relayPosition = 0;
  digitalWrite(pins::RELAY, relayPosition);
  firstScreenWrite = 1;
  tone(pins::BUZZER, 800, 300);
  display.drawBox(85, 29, 40, 14, ST7735_BLACK);
  display.textToScreenFull(89, 29, ST7735_RED, ST7735_BLACK, 2, F("Off"), false);
  return;
}

void OutputManager::setVoltage(float targetVoltage)
{
  float percentage = targetVoltage / characteristics::MAX_VOLTAGE;
  pwmValue = percentage * 255;
  analogWrite(pins::VOLTAGE_CONTROL, pwmValue);
}