#include "Arduino.h"
#include "tempcontroller.h"
#include "constants.h"
#include "variables.h"
#include "secondaryfunctions.h"


int tempControl()
{
  if(!tempSensorFailed)
  {
    byte temp;
    temp = analogRead(tempSensorPin);
    temp = temp * 0.48828125;
    if((temp >= 85) && (selfTestActive == false))   errorHandler(overheatCode);
    if((temp >= 150) && (selfTestActive == false))  errorHandler(temperatureSensorFailCode);
    if((temp >= 150) && (selfTestActive == true))   return 1;

    if(temp < 35) analogWrite(fanPin, 0);
    if((temp >= 35) && (temp < 40)) analogWrite(fanPin, 70);
    if((temp >= 40) && (temp < 50)) analogWrite(fanPin, 120);
    if((temp >= 50) && (temp < 60)) analogWrite(fanPin, 150);
    if((temp >= 60) && (temp < 70)) analogWrite(fanPin, 180);
    if((temp >= 70) && (temp < 85)) analogWrite(fanPin, 255);
    return temp;
    
  }
  return 0;
}