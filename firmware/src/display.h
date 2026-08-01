#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

extern Adafruit_ST7735 tft;

void initDisplay();
void textToScreenFull(int x, int y, uint16_t textColor, uint16_t backgroundColor, int size, const char *text, bool textWrap);
void textToScreenFast(int x, int y, const char *text, bool ignorePos);
void lineToScreen(int startX, int startY, int endX, int endY, uint16_t color);
void menuGridLines();
void settingsMenuTextData();
void settingsMenuText();
void powerSupplyModeLines();
void tunixBadge();
void mainMenuText();
void basicModScreenManager(float volt, float current);
void advancedModScreenManager(float volt, float current);

#endif