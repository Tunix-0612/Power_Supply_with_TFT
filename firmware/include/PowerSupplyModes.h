#ifndef POWERSUPPLYMODES_H
#define POWERSUPPLYMODES_H

#include <Arduino.h>

class PowerSupplyClass
{
  private:
    void setupAdvancedLayout();
    void advancedModRenderer(float volt, float current, bool relayActive);
  
  public:
    PowerSupplyClass();

    int standbyMode();
    void basicMod();
    void questVoltChangeScreenLines(char *questVoltStr);
    void questVoltChange();
    void advancedMod();

    const unsigned long refreshTime = 200;
    const unsigned long secondaryRefreshTime = 500;
};

extern PowerSupplyClass powerSupply;

#endif