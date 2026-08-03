#include "DisplayDriver.h"
#include "Constants.h"
#include "TunixMemoryManager.h"

DisplayDriver::DisplayDriver() : tft(pins::TFT_CS, pins::TFT_DC, -1) {}

void DisplayDriver::initDisplay()
{
  tft.initR(INITR_144GREENTAB);
  tft.setSPISpeed(8000000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(0);
  digitalWrite(pins::SCREEN_BACKLIGHT, HIGH);
}

//-----------------------------------
//  PRIMITIVE FUNCTIONS
//-----------------------------------

void DisplayDriver::textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const char *text, bool textWrap)
{
  tft.setCursor(x, y);
  tft.setTextColor(textColor, backgroundColor);
  tft.setTextSize(size);
  tft.setTextWrap(textWrap);
  tft.print(text);
}

void DisplayDriver::textToScreenFast(int x, int y, const char *text, bool ignorePos)
{
  if(!ignorePos) tft.setCursor(x, y);
  tft.print(text);
}

void DisplayDriver::lineToScreen(int startX, int startY, int endX, int endY, uint16_t color) { tft.drawLine(startX, startY, endX, endY, color); }

void DisplayDriver::clearScreen(uint16_t color = ST7735_BLACK) { tft.fillScreen(color); }

void DisplayDriver::drawBox(int x, int y, int w, int h, uint16_t color, bool filled)
{
  if(filled) tft.fillRect(x, y, w, h, color);
  else       tft.drawRect(x, y, w, h, color);
}

void DisplayDriver::drawCircleShape(int x, int y, int r, uint16_t color, bool filled)
{
  if(filled) tft.fillCircle(x, y, r, color);
  else       tft.drawCircle(x, y, r, color);
}

void DisplayDriver::drawTriangleShape(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, bool filled)
{
  if(filled) tft.fillTriangle(x1, y1, x2, y2, x3, y3, color);
  else       tft.drawTriangle(x1, y1, x2, y2, x3, y3, color);
}

//-----------------------------------
//  GUI FUNCTIONS
//-----------------------------------

void DisplayDriver::menuGridLines()
{
  lineToScreen(12, 34, 116, 34, TFT_GRAY);
  lineToScreen(12, 51, 116, 51, TFT_GRAY);
  lineToScreen(12, 68, 116, 68, TFT_GRAY);
  lineToScreen(12, 85, 116, 85, TFT_GRAY);
  lineToScreen(12, 102, 116, 102, TFT_GRAY);
}

void DisplayDriver::settingsMenuTextData()
{
  char currentLimitStr[10];
  dtostrf(memory.settings.currentLimit, 5, 2, currentLimitStr);
  if(memory.settings.currentLimit >= 10) textToScreenFull(94, 40, ST7735_WHITE, ST7735_BLACK, 1, currentLimitStr, false);
  else textToScreenFull(100, 40, ST7735_WHITE, ST7735_BLACK, 1, currentLimitStr, false);

  if(memory.settings.currentProtectionMode == true) textToScreenFast(112, 57, "CC", false);
  if(memory.settings.currentProtectionMode == false) textToScreenFast(112, 57, "CP", false);
  tft.setCursor(94, 74);

  if(memory.settings.batteryMode == true) textToScreenFast(94, 74, "Active", false); 
  if(memory.settings.batteryMode == false) textToScreenFast(92, 74, "Passive", false);
}

void DisplayDriver::settingsMenuText()
{
  textToScreenFull(5, 23, ST7735_WHITE, ST7735_BLACK, 1, "Volt Tolerance", false);
  textToScreenFast(5, 40, "Current Limitation", false);
  textToScreenFast(5, 74, "Battery Mode", false);
  textToScreenFast(5, 91, "Factory Reset", false);
  textToScreenFast(5, 108, "Back", false);
}


void DisplayDriver::powerSupplyModeLines()
{
  tft.fillScreen(ST7735_BLACK);
  lineToScreen(0, 27, 82, 27, TFT_GRAY);
  lineToScreen(0, 52, 82, 52, TFT_GRAY);
  lineToScreen(0, 77, 82, 77, TFT_GRAY);
  lineToScreen(0, 101, 128, 101, TFT_GRAY);
  lineToScreen(0, 105, 128, 105, TFT_GRAY);
  lineToScreen(42, 105, 42, 128, TFT_GRAY);
  lineToScreen(86, 105, 86, 128, TFT_GRAY);
  lineToScreen(82, 0, 82, 101, TFT_GRAY);
  textToScreenFull(95, 114, TFT_GRAY, ST7735_BLACK, 1, "+-Vo", false);
  tft.fillTriangle(10, 117, 28, 112, 28, 122, TFT_GRAY);
  tft.fillCircle(64, 117, 7, TFT_GRAY);
}

void DisplayDriver::tunixBadge()
{
  textToScreenFull(22, 3, TFT_GRAY, ST7735_BLACK, 1, "T", false);
  textToScreenFast(42, 3, "U", false);
  textToScreenFast(62, 3, "N", false);
  textToScreenFast(82, 3, "I", false);
  textToScreenFast(102, 3, "X", false);
  lineToScreen(0, 14, 128, 14, TFT_GRAY);
}

void DisplayDriver::mainMenuText()
{
  textToScreenFull(34, 23, ST7735_WHITE, ST7735_BLACK, 1 , "Basic Mode", false);
  textToScreenFast(25, 40, "Advanced Mode", false);
  textToScreenFast(40, 57, "Settings", false);
  textToScreenFast(31, 74, "Information", false);
}

void DisplayDriver::basicModScreenManager(float volt, float current)
{
  /*barValue = map(potValue, 0, 1024, 0, 128);
  tft.fillRect(0, 102, 128, 3, tft.color565(153, 255, 255));
  tft.fillRect(0, 102, barValue, 3, tft.color565(153, 255, 255));
  if (volt < 0) volt = 0;
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.setCursor(4, 10);
  if (volt < 10) 
  {
    dtostrf(volt, 5, 3, char *s)
  }
  else if(volt >=10) tft.print(volt, 2);
  tft.print("V");
  tft.setCursor(4, 35);
  if((currentCalculated < 10))
  {
    tft.print(currentCalculated, 3);
    tft.print("A");
  }
  else
  {
    tft.print(currentCalculated, 2);
    tft.print("A");
  }
  watt = volt * currentCalculated;
  tft.setCursor(4, 60);
  if(watt < 10) tft.print(watt, 3);
  else if (watt < 100) tft.print(watt, 2);
  else tft.print(watt, 1);
  tft.print("W");
  if(relayPosition == 1) tft.fillCircle(64, 117, 7, ST7735_GREEN);
  if(relayPosition == 0) tft.fillCircle(64, 117, 7, TFT_GRAY);*/
}

void DisplayDriver::advancedModScreenManager(float volt, float current)
{
  if (firstScreenWrite == true)
  {
    barValue = map(memory.settings.questVolt, 0, characteristics::MAX_VOLTAGE, 0, 128);
    char questVoltStr[10];
    if (memory.settings.questVolt >= 10)
    {
      dtostrf(memory.settings.questVolt, 5, 2, questVoltStr);
      textToScreenFull(4, 6, ST7735_WHITE, ST7735_BLACK, 2, questVoltStr, false);
      tft.print(memory.settings.questVolt, 2);
    }
    else
    {
      dtostrf(memory.settings.questVolt, 5, 3, questVoltStr);
      textToScreenFull(4, 6, ST7735_WHITE, ST7735_BLACK, 2, questVoltStr, false);
    }
  textToScreenFast(1, 1, "V", true);
  }
  //VOLT
  if ((volt != oldVolt) || (firstScreenWrite == true))
  {
    oldVolt = volt;
    char voltStr[10];
    if (volt < 10)  dtostrf(volt, 5, 3, voltStr);
    else            dtostrf(volt, 5, 2, voltStr);

    if (firstScreenWrite == true) 
    {
      textToScreenFull(4, 33, ST7735_WHITE, ST7735_BLACK, 2, voltStr, false);
      textToScreenFast(1, 1, "V", true);
    }
    else textToScreenFast(4, 33, voltStr, false);
  }
  //OVER CURRENT
  bool currentLimitExceeded = (currentCalculated > memory.settings.currentLimit);
  if (currentLimitExceeded != lastLimitState || firstScreenWrite == true)
  {
    lastLimitState = currentLimitExceeded;
    if (currentLimitExceeded) tft.drawRect(0, 53, 82, 24, ST7735_RED);
    else 
    {
      tft.drawRect(0, 53, 82, 24, ST7735_BLACK);
      tft.fillRect(85, 85, 43, 7, ST7735_BLACK);
    }
  }
  //CURRENT
  if ((currentCalculated != oldCurrent) || (firstScreenWrite == true)) 
  {
    oldCurrent = currentCalculated;
    char currentStr[10];
    if (currentCalculated < 10 && currentCalculated >= 0) dtostrf(currentCalculated, 5, 3, currentStr);
    else                                                  dtostrf(currentCalculated, 5, 2, currentStr);

    if (firstScreenWrite == true) 
    {
      textToScreenFull(4, 57, ST7735_WHITE, ST7735_BLACK, 2, currentStr, false);
      textToScreenFull(64, 57, ST7735_WHITE, ST7735_BLACK, 2, "A", false);
    } 
    else textToScreenFast(4, 57, currentStr, false);
  }
  //WATT
  watt = volt * currentCalculated;
  if ((watt != oldWatt) || (firstScreenWrite == 1)) 
  {
    oldWatt = watt;
    char wattStr[10];
    if (watt < 10)        dtostrf(watt, 5, 3, wattStr);
    else if (watt < 100)  dtostrf(watt, 5, 2, wattStr);
    else                  dtostrf(watt, 5, 1, wattStr);
    
    if (firstScreenWrite == 1) 
    {
      textToScreenFull(4, 82, ST7735_WHITE, ST7735_BLACK, 2, wattStr, false);
      textToScreenFull(64, 82, ST7735_WHITE, ST7735_BLACK, 2, "W", false);
    } 
    else textToScreenFast(4, 82, wattStr, false);
  }

  //RELAY
  static int lastRelayPosition = -1;
  if (relayPosition != lastRelayPosition || firstScreenWrite == 1)
  {
    lastRelayPosition = relayPosition;
    if (relayPosition == 1) tft.fillCircle(64, 117, 7, ST7735_GREEN);
    else                    tft.fillCircle(64, 117, 7, TFT_GRAY);
  }
}