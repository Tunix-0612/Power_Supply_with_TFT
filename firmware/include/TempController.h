#ifndef TEMPCONTROLLER_H
#define TEMPCONTROLLER_H

#include <Arduino.h>

class TempController
{
    private:

    public:
        TempController();

        int tempControl();

        bool tempSensorFailed = false;
};

extern TempController temperature;

#endif