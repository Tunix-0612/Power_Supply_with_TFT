#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef NO_GFX_FONTS
#define NO_GFX_FONTS // Disable GFX built-in fonts to save flash space
#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// --- DISPLAY COLORS ---
#define TFT_GRAY 0x7BEF

enum UISlot {
    SLOT_VOLTAGE,
    SLOT_CURRENT,
    SLOT_POWER,

    SLOT_SET_VOLTAGE,
    SLOT_STATUS_A,
    SLOT_STATUS_B,
    SLOT_STATUS_C,
    SLOT_TEMP_WARN,

    SLOT_BOTTOM_INFO
};

struct SlotConfig {
    int16_t x;
    int16_t y;
    uint8_t textSize;
    uint8_t maxLen;
    char text[12];
};

class DisplayDriver
{
  private:
    Adafruit_ST7735 tft;

  public:
    DisplayDriver();

    Adafruit_ST7735& getRawDisplay();
    
    void initDisplay();
    // ------------------------------------------------------------------------
    // 1. RAM Variation
    // ------------------------------------------------------------------------
    void textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const char *text, bool textWrap = false);
    void textToScreenFast(int x, int y, const char *text, bool ignorePos = false);

    // ------------------------------------------------------------------------
    // 2. FLASH Variation
    // ------------------------------------------------------------------------
    void textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const __FlashStringHelper *text, bool textWrap = false);
    void textToScreenFast(int x, int y, const __FlashStringHelper *text, bool ignorePos = false);

    // ------------------------------------------------------------------------
    // Slot renderer for String (RAM and FLASH variations).
    // ------------------------------------------------------------------------
    void drawSlotText(UISlot slot, const char* text, uint16_t textColor, uint16_t bgColor = ST7735_BLACK);
    void drawSlotText(UISlot slot, const __FlashStringHelper* text, uint16_t textColor, uint16_t bgColor = ST7735_BLACK);
    
    // ------------------------------------------------------------------------
    // Slot renderer for float.
    // ------------------------------------------------------------------------
    void drawSlotFloat(UISlot slot, float val, uint8_t width, uint8_t prec, const char* unit, uint16_t textColor, uint16_t bgColor = ST7735_BLACK);

    // ------------------------------------------------------------------------
    // Header renderer.
    // ------------------------------------------------------------------------
    void drawHeader(const char* title, uint16_t textColor = ST7735_WHITE, uint16_t lineColor = TFT_GRAY);
    void drawHeader(const __FlashStringHelper* title, uint16_t textColor = ST7735_WHITE, uint16_t lineColor = TFT_GRAY);

    // ------------------------------------------------------------------------
    // Other renderers.
    // ------------------------------------------------------------------------
    void lineToScreen(int startX, int startY, int endX, int endY, uint16_t color);
    void drawBox(int x, int y, int w, int h, uint16_t color, bool filled = false);
    void drawCircleShape(int x, int y, int r, uint16_t color, bool filled = false);
    void drawTriangleShape(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, bool filled = false);
    void clearScreen(uint16_t color = ST7735_BLACK);
};

extern DisplayDriver display;

#endif