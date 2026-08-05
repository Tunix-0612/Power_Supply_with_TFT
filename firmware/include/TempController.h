#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <Arduino.h>

class TempController
{
  private:
    uint8_t currentFanPWM = 0;
    uint8_t currentAggression = 0;
        
    unsigned long highPowerStartTime = 0;
    bool isHighPowerActive = false;

    int readTemperature();

    uint8_t getPowerAggression(float currentPower);
    uint8_t getTempAggression(int currentTemp);

    void applyAggression(uint8_t level);

    public:
        TempController();

        void tempControl();

        bool tempSensorFailed = false;
};

extern TempController temperature;

#endif