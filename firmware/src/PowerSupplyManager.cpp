#include "PowerSupplyManager.h"

#include "Constants.h"
#include "DisplayDriver.h"
#include "TunixMemoryManager.h"
#include "TempController.h"
#include "OutputManager.h"
#include "InputManager.h"

byte questVoltChangeCursor;

PowerSupplyClass::PowerSupplyClass() { }

int PowerSupplyClass::standbyMode()
{
  output.togglePowerBoard(false);
  output.setRelay(false);
  output.setVoltage(0);

  display.toggleDisplay(false);

  unsigned long lastDeviceCheck = millis();

  while(true)
  {
    inputManager.update();

    if(inputManager.getEvent(BTN_UP) == BTN_EVENT_CLICK) break;

    if(millis() - lastDeviceCheck >= 1000)
    {
      lastDeviceCheck = millis();
      temperature.tempControl();
    }
  }

  display.toggleDisplay(true);

  output.readVoltage();
  output.readCurrent();
  output.readMainVoltage();
  
  output.togglePowerBoard(true);
  
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

void PowerSupplyClass::renderSetVoltageMenu(uint8_t cursor)
{
    display.drawBox(31, 75, 60, 2, ST7735_BLACK, true);

    char voltStr[10];
    dtostrf(memory.settings.questVolt, 5, 2, voltStr);

    display.textToScreenFull(31, 60, ST7735_WHITE, ST7735_BLACK, 2, voltStr, false);
    display.textToScreenFull(95, 60, ST7735_YELLOW, ST7735_BLACK, 2, F("V"), false);
    
    if (cursor == 1) display.getRawDisplay().drawFastHLine(31, 75, 24, TFT_GRAY);
    if (cursor == 2) display.getRawDisplay().drawFastHLine(67, 75, 10, TFT_GRAY);
    if (cursor == 3) display.getRawDisplay().drawFastHLine(79, 75, 10, TFT_GRAY);
}

void PowerSupplyClass::setVoltageMenu()
{
  display.clearScreen(ST7735_BLACK);
  display.drawHeader(F("SET VOLTAGE"));

  uint8_t setVoltageCursor = 1;
  float lastDrawnVolt = -1.0f;
  uint8_t lastDrawnCursor = 0;

  unsigned long buttonPressTimer = 0;
  bool isRepeating = false;
  unsigned long lastTempCheck = millis();

  while (setVoltageCursor <= 3)
  {
    inputManager.update();
    
    ButtonEvent eventUp    = inputManager.getEvent(BTN_UP);
    ButtonEvent eventLeft  = inputManager.getEvent(BTN_LEFT);
    ButtonEvent eventRight = inputManager.getEvent(BTN_RIGHT);

    if (eventUp == BTN_EVENT_CLICK) 
    {
      tone(pins::BUZZER, 1500, 20);
      setVoltageCursor++;
    }

    bool dec = (eventLeft == BTN_EVENT_CLICK);
    bool inc = (eventRight == BTN_EVENT_CLICK);

    if (digitalRead(pins::LEFT_BUTTON) == LOW) 
    {
        if (millis() - buttonPressTimer > (isRepeating ? 100 : 500)) 
        {
            dec = true;
            buttonPressTimer = millis();
            isRepeating = true;
        }
    } 
    else if (digitalRead(pins::RIGHT_BUTTON) == LOW) 
    {
        if (millis() - buttonPressTimer > (isRepeating ? 100 : 500)) 
        {
            inc = true;
            buttonPressTimer = millis();
            isRepeating = true;
        }
    } 
    else 
    {
        isRepeating = false;
        if (!inc && !dec) buttonPressTimer = millis();
    }

    if (dec || inc) 
    {
        tone(pins::BUZZER, 1500, 10);
        float multiplier = (dec) ? -1.0f : 1.0f;

        if (setVoltageCursor == 1) memory.settings.questVolt += (1.00f * multiplier);
        if (setVoltageCursor == 2) memory.settings.questVolt += (0.10f * multiplier);
        if (setVoltageCursor == 3) memory.settings.questVolt += (0.01f * multiplier);

        memory.settings.questVolt = constrain(memory.settings.questVolt, 0.0f, characteristics::MAX_VOLTAGE);

        memory.settings.questVolt = round(memory.settings.questVolt * 100.0f) / 100.0f;
    }

    if (memory.settings.questVolt != lastDrawnVolt || setVoltageCursor != lastDrawnCursor)
    {
        renderSetVoltageMenu(setVoltageCursor);
        lastDrawnVolt = memory.settings.questVolt;
        lastDrawnCursor = setVoltageCursor;
    }

    if (millis() - lastTempCheck >= 1000)
    {
      lastTempCheck = millis();
      temperature.tempControl();
    }
  }
  memory.saveBasicMemory();
  tone(pins::BUZZER, 1800, 50);
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
  output.setRelay(false); 
  bool outputActive = false;
  
  unsigned long previousTime = 0;
  unsigned long secondaryPreviousTime = 0;
  unsigned long currentTime = 0;

  setupAdvancedLayout();

  previousTime = millis();
  secondaryPreviousTime = millis();

  while(true)
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

      setVoltageMenu();

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

  output.setRelay(false);
  output.setVoltage(0);

  inputManager.update();
  display.clearScreen(ST7735_BLACK);
  tone(pins::BUZZER, 1800, 50);
  return;
}