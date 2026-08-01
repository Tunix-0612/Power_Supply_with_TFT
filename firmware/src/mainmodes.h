#ifndef MAINMODES_H
#define MAINMODES_H

#include <Arduino.h>

int standbyMode();
void basicMod();
void questVoltChangeScreenLines(char *questVoltStr);
void questVoltChange();
void advancedMod();

#endif