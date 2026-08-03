#ifndef POWERSUPPLYMODES_H
#define POWERSUPPLYMODES_H

#include <Arduino.h>

class PowerSupplyClass
{
  private:
  
  public:
    PowerSupplyClass();

    int standbyMode();
    void basicMod();
    void questVoltChangeScreenLines(char *questVoltStr);
    void questVoltChange();
    void advancedMod();

    unsigned long previousTime = 0;
    unsigned long secondaryPreviousTime = 0;
    unsigned long currentTime = 0;
    const unsigned long refreshTime = 100;
    const unsigned long secondaryRefreshTime = 500;
};

extern PowerSupplyClass powerSupply;

#endif