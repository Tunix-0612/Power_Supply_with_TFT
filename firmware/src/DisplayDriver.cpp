#include "DisplayDriver.h"

#include "Constants.h"
#include "TunixMemoryManager.h"

SlotConfig slots[] =  
{
    [SLOT_VOLTAGE]     = { .x = 4,  .y = 33, .textSize = 2, .maxLen = 7 },  // Example: "12.50 V"
    [SLOT_CURRENT]     = { .x = 4,  .y = 50, .textSize = 2, .maxLen = 7 },  // Example: " 2.10 A"
    [SLOT_POWER]       = { .x = 4,  .y = 80, .textSize = 2, .maxLen = 10},  // Example: " 26.25 W "

    [SLOT_SET_VOLTAGE] = { .x = 85, .y = 50, .textSize = 1, .maxLen = 5 }
};

DisplayDriver::DisplayDriver() : tft(pins::TFT_CS, pins::TFT_DC, -1) { }

Adafruit_ST7735& DisplayDriver::getRawDisplay() { return tft; }

void DisplayDriver::initDisplay()
{
  tft.initR(INITR_144GREENTAB);
  tft.setSPISpeed(8000000);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(0);
  digitalWrite(pins::SCREEN_BACKLIGHT, HIGH);
}

// ============================================================================
// Flash variation for text rendering
// ============================================================================
void DisplayDriver::textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const __FlashStringHelper *text, bool textWrap)
{
  tft.setCursor(x, y);
  tft.setTextColor(textColor, backgroundColor);
  tft.setTextSize(size);
  tft.setTextWrap(textWrap);
  tft.print(text); 
}

void DisplayDriver::textToScreenFast(int x, int y, const __FlashStringHelper *text, bool ignorePos)
{
  if(!ignorePos) tft.setCursor(x, y);
  tft.print(text);
}

// ============================================================================
// RAM variation for text rendering
// ============================================================================
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

// ============================================================================
// PRINT STRING TO SCREEN SLOT
// ============================================================================
void DisplayDriver::drawSlotText(UISlot slot, const char* text, uint16_t textColor, uint16_t bgColor) 
{
    SlotConfig &cfg = slots[slot];

    if (strncmp(cfg.text, text, cfg.maxLen) == 0) return;

    
    strncpy(cfg.text, text, cfg.maxLen);
    cfg.text[cfg.maxLen] = '\0';

    tft.setCursor(cfg.x, cfg.y);
    tft.setTextSize(cfg.textSize);
    tft.setTextColor(textColor, bgColor);
    tft.print(text);

    uint8_t len = strlen(text);
    while (len < cfg.maxLen) 
    {
        tft.print(' ');
        len++;
    }
}

// ============================================================================
// CONVERT FLASH STRING TO RAM STRING AND CALL THE RAM VERSION OF drawSlotText
// ============================================================================
void DisplayDriver::drawSlotText(UISlot slot, const __FlashStringHelper* text, uint16_t textColor, uint16_t bgColor) 
{
    char tempBuf[16];

    // Copy the Flash string into the temporary buffer
    PGM_P p = reinterpret_cast<PGM_P>(text);
    strncpy_P(tempBuf, p, sizeof(tempBuf) - 1);
    tempBuf[sizeof(tempBuf) - 1] = '\0';

    // Now call the RAM version of drawSlotText with the copied string
    drawSlotText(slot, tempBuf, textColor, bgColor);
}

// ============================================================================
// CONVERT FLOAT TO STRING AND CALL THE RAM VERSION OF drawSlotText
// ============================================================================
void DisplayDriver::drawSlotFloat(UISlot slot, float val, uint8_t width, uint8_t prec, const char* unit, uint16_t textColor, uint16_t bgColor) 
{
    char buf[16];
    char floatBuf[10];

    dtostrf(val, width, prec, floatBuf);
    snprintf(buf, sizeof(buf), "%s%s", floatBuf, unit);

    drawSlotText(slot, buf, textColor, bgColor);
}

// ----------------------------------------------------------------------------
// RAM Header renderer
// ----------------------------------------------------------------------------
void DisplayDriver::drawHeader(const char* title, uint16_t textColor, uint16_t lineColor) 
{
    uint8_t len = strlen(title);
    int16_t xPos = (128 - (len * 6)) / 2;
    if (xPos < 0) xPos = 0;

    textToScreenFull(xPos, 4, textColor, ST7735_BLACK, 1, title, false);

    tft.drawFastHLine(0, 16, 128, lineColor);
}

// ----------------------------------------------------------------------------
// FLASH Header renderer
// ----------------------------------------------------------------------------
void DisplayDriver::drawHeader(const __FlashStringHelper* title, uint16_t textColor, uint16_t lineColor) 
{
    char tempBuf[20];
    PGM_P p = reinterpret_cast<PGM_P>(title);
    strncpy_P(tempBuf, p, sizeof(tempBuf) - 1);
    tempBuf[sizeof(tempBuf) - 1] = '\0';

    drawHeader(tempBuf, textColor, lineColor);
}

void DisplayDriver::lineToScreen(int startX, int startY, int endX, int endY, uint16_t color) { tft.drawLine(startX, startY, endX, endY, color); }

void DisplayDriver::clearScreen(uint16_t color) { tft.fillScreen(color); }

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

void DisplayDriver::toggleDisplay(bool state)
{
  uint8_t targetPWM = map(memory.settings.backlightLevel, 0, 100, 0, 255);
  uint8_t fadeDelay = 5;

  if (state == true) 
  {
    for (int i = 0; i <= targetPWM; i += 5) 
    {
      analogWrite(pins::SCREEN_BACKLIGHT, i);
      delay(fadeDelay);
    }
    analogWrite(pins::SCREEN_BACKLIGHT, targetPWM);
  }
  else 
  {
    for (int i = targetPWM; i >= 0; i -= 5) 
    {
      analogWrite(pins::SCREEN_BACKLIGHT, i);
      delay(fadeDelay);
    }
    analogWrite(pins::SCREEN_BACKLIGHT, 0);
  }
  display.clearScreen();
}