#ifndef VARIABLES_H
#define VARIABLES_H

#include <Arduino.h>


extern byte temperature;
extern byte pwmValue;

extern bool relayPosition, firstScreenWrite;
extern bool tempSensorFailed, lowVoltageAcknowledged;
extern bool selfTestActive;

extern float resistorUp, resistorMainUp;
extern float resistorDown, resistorMainDown;
extern float voltCalculated, voltMainCalculated;

extern float currentCalculated;
extern float shuntResistor; 
extern float gain;

extern unsigned long currentTime, previousTime, secondaryPreviousTime;

extern int timeOut,  refreshTime, secondaryRefreshTime;
extern int potValue, barValue;


struct versionStruct
{
  byte majorVersion;
  byte minorVersion;
  byte patchVersion;
};

struct settingStruct
{
  float voltTolerance;
  float currentLimit;
  bool currentProtectionMode;
  bool batteryMode;
  float questVolt;
};

extern versionStruct deviceVersion;
extern settingStruct settings;
extern const settingStruct defaultSettings;

#endif