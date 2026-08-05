#include "PowerSupplyModes.h"

#include "Constants.h"
#include "DisplayDriver.h"
#include "TunixMemoryManager.h"
#include "TempController.h"
#include "DisplayDriver.h"
#include "OutputManager.h"
#include "InputManager.h"

byte questVoltChangeCursor;

PowerSupplyClass::PowerSupplyClass() { }

int PowerSupplyClass::standbyMode()
{
  digitalWrite(pins::POWER_CONTROL, LOW);
  digitalWrite(pins::VOLTAGE_CONTROL, LOW);
  for (int counter = 255; counter != 0; counter--) 
  {
    analogWrite(pins::SCREEN_BACKLIGHT, counter);
    delay(5);
  }
  display.clearScreen();
  while(digitalRead(pins::UP_BUTTON) == HIGH)
  {
    delay(2000);
    temperature.tempControl();
  }
  digitalWrite(pins::SCREEN_BACKLIGHT, HIGH);
  digitalWrite(pins::POWER_CONTROL, HIGH);
  digitalWrite(pins::VOLTAGE_CONTROL, HIGH);
  tone(pins::BUZZER, 1800 , 50);
  return 0;
}

void PowerSupplyClass::basicMod()
{
  /*digitalWrite(pins::RELAY, LOW);
  display.relayPosition = 0;
  previousTime = currentTime;
  secondaryPreviousTime = currentTime;
  basicModLines();
  while(digitalRead(topButtonPin) == HIGH);
  while(digitalRead(leftButtonPin) == LOW)
  {
    currentTime = millis();
    voltControl();
    potValue = analogRead(potPin);
    pwmValue = map(potValue, 0, 1023, 0, 253);
    analogWrite(voltageControlPin, pwmValue);
    if(digitalRead(rightButtonPin) == HIGH) 
    {
      relayChange();
    }
    if(current > settings.currentLimit)
    {
      delay(50);
      if(current > settings.currentLimit) relayOff();
    }
    if(currentTime - previousTime >= refreshTime)
    {
      previousTime = currentTime;
      basicModScreenManager(voltCalculated, currentCalculated);
      tempControl();
    }
  }
  digitalWrite(pins::RELAY, LOW);
  display.relayPosition = false;
  while(digitalRead(pins::LEFT_BUTTON) == LOW);
  tone(pins::BUZZER, 1800, 50);
  return;*/
}

void PowerSupplyClass::questVoltChangeScreenLines(char *questVoltStr)
{
  display.textToScreenFull(31, 60, ST7735_WHITE, ST7735_BLACK, 2, questVoltStr, false);
  display.textToScreenFast(1, 1, F("V"), true);
  if(questVoltChangeCursor == 1)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(43, 75, 52, 75, TFT_GRAY);
    else                          display.lineToScreen(31, 75, 40, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 2)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(67, 75, 76, 75, TFT_GRAY);
    else                          display.lineToScreen(55, 75, 64, 75, TFT_GRAY);
  }
  if(questVoltChangeCursor == 3)
  {
    if(memory.settings.questVolt >= 10)  display.lineToScreen(79, 75, 88, 75, TFT_GRAY);
    else                          display.lineToScreen(67, 75, 76, 75, TFT_GRAY);
  }
}

void PowerSupplyClass::questVoltChange()
{
  display.clearScreen();
  display.textToScreenFull(20, 5, ST7735_WHITE, ST7735_BLACK, 2, F("Quest V"), false);
  display.lineToScreen(0, 25, 128, 25, TFT_GRAY);
  questVoltChangeCursor = 1;
  delay(100);
  char questVoltStr[10];
  dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
  questVoltChangeScreenLines(questVoltStr);
  while(questVoltChangeCursor <= 3)
  {
    if(digitalRead(pins::UP_BUTTON) == LOW) 
    {
      tone(pins::BUZZER, 1500, 20);
      while(digitalRead(pins::UP_BUTTON) == LOW);
      questVoltChangeCursor++;
      questVoltChangeScreenLines(questVoltStr);
    }
    while(digitalRead(pins::LEFT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      if(questVoltChangeCursor == 1) memory.settings.questVolt -= 1;
      if(questVoltChangeCursor == 2) memory.settings.questVolt -= 0.1;
      if(questVoltChangeCursor == 3) memory.settings.questVolt -= 0.01;
      memory.settings.questVolt = constrain(memory.settings.questVolt, 0, characteristics::MAX_VOLTAGE);
      dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
    while(digitalRead(pins::RIGHT_BUTTON) == LOW)
    {
      tone(pins::BUZZER, 1500, 20);
      if(questVoltChangeCursor == 1) memory.settings.questVolt += 1;
      if(questVoltChangeCursor == 2) memory.settings.questVolt += 0.1;
      if(questVoltChangeCursor == 3) memory.settings.questVolt += 0.01;
      memory.settings.questVolt = constrain(memory.settings.questVolt, 0, characteristics::MAX_VOLTAGE);
      dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
      questVoltChangeScreenLines(questVoltStr);
      delay(250);
    }
  }
  questVoltChangeCursor = 1;
  EEPROM.put(SETTINGS_ADRESS, memory.settings);
  tone(pins::BUZZER, 1800, 50);
  return;
}

void PowerSupplyClass::setupAdvancedLayout() 
{
    display.clearScreen(ST7735_BLACK);
    display.getRawDisplay().drawFastHLine(72, 22, 56, TFT_GRAY);
    display.getRawDisplay().drawFastHLine(72, 44, 56, TFT_GRAY);
    display.getRawDisplay().drawFastHLine(0,  66, 128, TFT_GRAY);
}

void PowerSupplyClass::advancedModRenderer(float volt, float current, bool relayActive)
{
    // --- LEFT SLOTS ---

    // Voltage Render.
    uint8_t voltPrec = (volt >= 10.0f) ? 2 : 3;
    display.drawSlotFloat(SLOT_VOLTAGE, volt, 5, voltPrec, "V", ST7735_WHITE);

    // Current Render.
    uint8_t currPrec = (current >= 10.0f) ? 2 : 3;
    display.drawSlotFloat(SLOT_CURRENT, current, 5, currPrec, "A", ST7735_YELLOW);

    // Watt Render.
    float watt = volt * current;
    uint8_t wattPrec = (watt >= 100.0f) ? 1 : ((watt >= 10.0f) ? 2 : 3);
    display.drawSlotFloat(SLOT_POWER, watt, 5, wattPrec, "W", ST7735_CYAN);


    // --- RIGHT SLOTS ---

    // Set Volt Render.
    uint8_t setVoltPrec = (memory.settings.questVolt >= 10.0f) ? 1 : 2;
    display.drawSlotFloat(SLOT_SET_VOLTAGE, memory.settings.questVolt, 4, setVoltPrec, "V", ST7735_GREEN);

    // Operation Mode Render.
    if (current > memory.settings.currentLimit) display.drawSlotText(SLOT_STATUS_A, F("MODE: OCP"), ST7735_RED);
    else 
    {
        if (memory.settings.currentProtectionMode == 1) display.drawSlotText(SLOT_STATUS_A, F("MODE: CC "), ST7735_GREEN);
        else                                            display.drawSlotText(SLOT_STATUS_A, F("MODE: CV "), ST7735_GREEN);
    }

    // Output Status Render.
    if (relayActive)  display.drawSlotText(SLOT_STATUS_B, F("OUT: ON "), ST7735_GREEN);
    else              display.drawSlotText(SLOT_STATUS_B, F("OUT: OFF"), TFT_GRAY);
}

void PowerSupplyClass::advancedMod()
{
  digitalWrite(pins::RELAY, LOW);
  bool outputActive = false;
  
  unsigned long previousTime = 0;
  unsigned long secondaryPreviousTime = 0;
  unsigned long currentTime = 0;

  setupAdvancedLayout();

  previousTime = millis();
  secondaryPreviousTime = millis();

  while(digitalRead(pins::LEFT_BUTTON) == HIGH)
  {
    currentTime = millis();

    float voltCalculated = output.readVoltage();
    float voltMainCalculated = output.readMainVoltage();
    float currentCalculated = output.readCurrent();

    inputManager.update();

    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);
    ButtonEvent eventRight = inputManager.getEvent(BTN_RIGHT);
    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventDown  = inputManager.getEvent(BTN_DOWN);
    
    if(eventRight == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 1500, 20);
      digitalWrite(pins::RELAY, LOW);
      outputActive = false;

      questVoltChange();

      setupAdvancedLayout();
    }
    if(eventUp == BTN_EVENT_CLICK)
    {
      tone(pins::BUZZER, 2000, 30);
      outputActive = !outputActive;

      if(outputActive) 
      {
        digitalWrite(pins::RELAY, HIGH);
        output.setVoltage(memory.settings.questVolt);
      } 
      else 
      {
        digitalWrite(pins::RELAY, LOW);
        output.setVoltage(0);
      }
    }
    if(currentTime - previousTime >= refreshTime)
    {
      //Main Manager
      previousTime = currentTime;
      advancedModRenderer(voltCalculated, currentCalculated, outputActive);
    }

    if(currentTime - secondaryPreviousTime >= secondaryRefreshTime)
    {
      //Secondary Manager
      secondaryPreviousTime = currentTime;
      temperature.tempControl();
    }
  }
  digitalWrite(pins::RELAY, LOW);
  outputActive = false;
  inputManager.update();
  display.clearScreen(ST7735_BLACK);
  tone(pins::BUZZER, 1800, 50);
  return;
}