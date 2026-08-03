#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

class MenuClass
{
  private:
    byte menuCursor, sleepTime;
    int returnTime;

  public:
    MenuClass();

    void menuSelectionCursor();
    void infoMenu();
    void settingsMenu();
    void mainMenu();

    unsigned long currentTime, previousTime, secondaryPreviousTime;

    int timeOut, refreshTime = 250, secondaryRefreshTime = 2500;
    int potValue, barValue;
};

extern MenuClass menu;

#endif