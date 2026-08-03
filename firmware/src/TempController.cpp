#include "TempController.h"
#include "Constants.h"
#include "TunixErrorManager.h"

TempController::TempController() { }

int TempController::tempControl()
{
  if(!tempSensorFailed)
  {
    bool selfTestActive = false;
    byte temp;
    temp = analogRead(pins::TEMP_SENSOR);
    temp = temp * 0.48828125;
    if((temp >= 85) && (selfTestActive == false))   errorManager.errorHandler(ErrorCode::OVERHEAT);
    if((temp >= 150) && (selfTestActive == false))  errorManager.errorHandler(ErrorCode::TEMP_SENSOR_FAIL);
    if((temp >= 150) && (selfTestActive == true))   return 1;

    if(temp < 35) analogWrite(pins::FAN, 0);
    if((temp >= 35) && (temp < 40)) analogWrite(pins::FAN, 70);
    if((temp >= 40) && (temp < 50)) analogWrite(pins::FAN, 120);
    if((temp >= 50) && (temp < 60)) analogWrite(pins::FAN, 150);
    if((temp >= 60) && (temp < 70)) analogWrite(pins::FAN, 180);
    if((temp >= 70) && (temp < 85)) analogWrite(pins::FAN, 255);
    return temp;
    
  }
  return 0;
}