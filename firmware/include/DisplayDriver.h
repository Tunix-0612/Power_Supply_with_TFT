#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef NO_GFX_FONTS
#define NO_GFX_FONTS // Özel font desteğini ve motorunu tamamen devre dışı bırakır
#endif

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class DisplayDriver
{
  private:
    Adafruit_ST7735 tft;

    float oldVolt, oldCurrent, watt, oldWatt;
    bool lastLimitState = false;

  public:
    DisplayDriver();
    
    void initDisplay();
    void textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const char *text, bool textWrap);
    void textToScreenFast(int x, int y, const char *text, bool ignorePos);
    void lineToScreen(int startX, int startY, int endX, int endY, uint16_t color);
    void drawBox(int x, int y, int w, int h, uint16_t color, bool filled = false);
    void drawCircleShape(int x, int y, int r, uint16_t color, bool filled = false);
    void drawTriangleShape(int x1, int y1, int x2, int y2, int x3, int y3, uint16_t color, bool filled = false);
    void clearScreen(uint16_t color = ST7735_BLACK);

    void menuGridLines();
    void settingsMenuTextData();
    void settingsMenuText();
    void powerSupplyModeLines();
    void tunixBadge();
    void mainMenuText();
    void basicModScreenManager(float volt, float current);
    void advancedModScreenManager(float volt, float current);

    bool firstScreenWrite = true;
    int barValue = 0;
    float voltCalculated = 0, currentCalculated = 0, voltMainCalculated = 0;
    bool relayPosition = false;
};

extern DisplayDriver display;

#endif