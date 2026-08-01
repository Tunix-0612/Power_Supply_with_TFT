#include "Adafruit_ST7735.h"
#include "poweroutputmanager.h"
#include "constants.h"
#include "variables.h"
#include "display.h"
#include "secondaryfunctions.h"


float readVoltage()
{
  long sum = 0;

  for(int i = 0; i < VOLTAGE_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(voltMeterPin);
    delay(1);
  }

  float average = sum / (float)VOLTAGE_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float volt = pinVolt * ((resistorUp + resistorDown) / resistorDown);

  return volt;
}

float readMainVoltage()
{
  long sum = 0;

  for(int i = 0; i < VOLTAGE_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(voltMeterMainPin);
    delay(1);
  }

  float average = sum / (float)VOLTAGE_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float volt = pinVolt * ((resistorMainUp + resistorMainDown) / resistorMainDown);
  
  if(volt <= 11 && volt >= 8 && volt == false) errorHandler(34);
  if(volt < 8) errorHandler(35);
  if(volt > 27) errorHandler(36);
  
  return volt;
}

float readCurrent()
{
  long sum = 0;

  for(int i = 0; i < CURRENT_SAMPLE_COUNT; i++) 
  {
    sum += analogRead(currentMeterPin);
    delay(1);
  }

  float average = sum / (float)CURRENT_SAMPLE_COUNT;
  float pinVolt = (average / 1023.0) * 5.0;
  float resistorVolt = pinVolt / gain;
  float current = resistorVolt / shuntResistor;

  return current;
}

void relayChange()
{
  tone(buzzerPin, 1800, 50);
  while (digitalRead(topButtonPin) == LOW);
  delay(100);
  relayPosition = !relayPosition;
  digitalWrite(relayPin, relayPosition);
  firstScreenWrite = 1;
  tft.fillRect(85, 29, 40, 14, ST7735_BLACK);

  if(relayPosition == true)   textToScreenFull(95, 29, ST7735_WHITE, ST7735_BLACK, 2, "On", false);
  if(relayPosition == false)  textToScreenFull(89, 29, ST7735_WHITE, ST7735_BLACK, 2, "Off", false);

  return;
}

void relayOff()
{
  relayPosition = 0;
  digitalWrite(relayPin, relayPosition);
  firstScreenWrite = 1;
  tone(buzzerPin, 800, 300);
  tft.fillRect(85, 29, 40, 14, ST7735_BLACK);
  textToScreenFull(89, 29, ST7735_RED, ST7735_BLACK, 2, "Off", false);
  return;
}

void setVoltage(float targetVoltage)
{
  float percentage = targetVoltage / MAX_VOLTAGE;
  pwmValue = percentage * 255;
  analogWrite(voltageControlPin, pwmValue);
}