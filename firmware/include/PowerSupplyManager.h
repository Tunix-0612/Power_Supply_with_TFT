#ifndef POWERSUPPLYMANAGER_H
#define POWERSUPPLYMANAGER_H

#include <Arduino.h>

class PowerSupplyClass
{
  private:
    void setupAdvancedLayout();
    void advancedModRenderer(float volt, float current, bool relayActive);
  
  public:
    PowerSupplyClass();

    const unsigned long refreshTime = 200;
    const unsigned long secondaryRefreshTime = 500;

    uint8_t powerState;

    int standbyMode();
    
    void basicMod();

    void renderSetVoltageMenu(uint8_t cursor);
    void setVoltageMenu();
    void advancedMod();
};

extern PowerSupplyClass powerSupply;

#endif