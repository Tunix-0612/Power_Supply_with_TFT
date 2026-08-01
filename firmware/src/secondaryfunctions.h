#ifndef SECONDARYFUNCTIONS_H
#define SECONDARYFUNCTIONS_H

#include <Arduino.h>
#include <avr/wdt.h>

void errorHandler(int errorCode);
void eepromReset(bool silent, bool setDefaults);

#endif