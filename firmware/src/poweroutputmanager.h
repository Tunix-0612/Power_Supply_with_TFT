#ifndef POWEROUTPUTMANAGER_H
#define POWEROUTPUTMANAGER_H

#include <Arduino.h>

float readVoltage();
float readMainVoltage();
float readCurrent();
void relayChange();
void relayOff();
void setVoltage(float targetVoltage);

#endif