#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <Arduino.h>

class OutputManager 
{
  private:
    float resistorUp = 10000, resistorMainUp;
    float resistorDown = 2000, resistorMainDown;
    float shuntResistor = 0.1;
    float gain = 3.0;

    float voltageMultiplier;
    float mainVoltageMultiplier;
    float currentMultiplier;
    float pwmMultiplier;

    bool outputActive = false;

    void calculateMultipliers();

  public:
    OutputManager();

    byte pwmValue;

    void updateResistors(float rUp, float rDown, float rMainUp, float rMainDown, float shunt, float g);

    float readVoltage();
    float readMainVoltage();
    float readCurrent();

    void toggleRelay();
    void setRelay(bool state);

    void setVoltage(float targetVoltage);

    void togglePowerBoard(bool state);
};

extern OutputManager output;

#endif