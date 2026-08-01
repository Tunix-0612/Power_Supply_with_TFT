#ifndef FIRMWAREANDSELFTEST_H
#define FIRMWAREANDSELFTEST_H

#include <Arduino.h>
#include <avr/wdt.h>

void firmwareCheck();
void selfTest(bool silent);

#endif