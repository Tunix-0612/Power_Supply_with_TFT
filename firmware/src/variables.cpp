#include "variables.h"


byte temperature;
byte pwmValue;

bool relayPosition = false, firstScreenWrite;
bool tempSensorFailed, lowVoltageAcknowledged;
bool selfTestActive;

float resistorUp = 10000, resistorMainUp;
float resistorDown = 2000, resistorMainDown;
float voltCalculated, voltMainCalculated;

float currentCalculated;
float shuntResistor = 0.1;
float gain = 3.0;

unsigned long currentTime, previousTime, secondaryPreviousTime;

int timeOut, refreshTime = 250, secondaryRefreshTime = 2500;
int potValue, barValue;

versionStruct deviceVersion;
settingStruct settings;

const settingStruct defaultSettings = 
{
  0.5,
  1.00,
  false,
  false,
  5.00
};