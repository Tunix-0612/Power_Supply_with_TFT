#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <Arduino.h>

class OutputManager 
{
  private:

  public:
    OutputManager();

    float readVoltage();
    float readMainVoltage();
    float readCurrent();
    void relayChange();
    void relayOff();
    void setVoltage(float targetVoltage);

    float resistorUp = 10000, resistorMainUp;
    float resistorDown = 2000, resistorMainDown;
    float voltCalculated, voltMainCalculated;
    float currentCalculated;
    float shuntResistor = 0.1;
    float gain = 3.0;

    bool relayPosition = false, firstScreenWrite;
    byte pwmValue;
};

extern OutputManager output;

#endif