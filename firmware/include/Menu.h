#ifndef MENU_H
#define MENU_H

#include <Arduino.h>

class MenuClass
{
  private:
    uint8_t menuCursor;

    void tunixBadge();

    void voltageSettingsMenu();
    void currentSettingsMenu();
    void backlightSettingsMenu();

  public:
    MenuClass();

    void menuSelectionCursor(uint8_t currentCursor, uint8_t previousCursor);
    void settingsMenu();
    void mainMenu();
};

extern MenuClass menu;

#endif