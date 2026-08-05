#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <Arduino.h>

class TempController
{
  private:
    uint8_t currentFanPWM = 0;            // Anlık uygulanan PWM değeri
    uint8_t currentAggression = 0;        // Anlık agresyon seviyesi (0, 1, 2, 3)
        
    unsigned long highPowerStartTime = 0; // 50W barajının aşıldığı an
    bool isHighPowerActive = false;       // Güç barajı aşıldı mı?

    int readTemperature();

    // Puanlama Fonksiyonları
    uint8_t getPowerAggression(float currentPower);
    uint8_t getTempAggression(int currentTemp);
        
    // Donanım Fonksiyonu
    void applyAggression(uint8_t level);

    public:
        TempController();

        void tempControl();

        bool tempSensorFailed = false;
};

extern TempController temperature;

#endif