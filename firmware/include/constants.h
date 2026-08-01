#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#include <SPI.h>

//Display Pins
#define TFT_CS 7
#define TFT_DC 8
#define TFT_MOSI 11
#define TFT_CLK 13

//Generic Pins
#define rightButtonPin 1
#define topButtonPin 2
#define bottomButtonPin 3
#define leftButtonPin 4
#define fanPin 5
#define screenBacklightPin 6
#define voltageControlPin 9
#define currentControlPin 10
#define buzzerPin 12

#define DTCPin A0
#define potPin A1
#define tempSensorPin A2
#define voltMeterMainPin A3
#define powerControlPin A4
#define relayPin A5
#define voltMeterPin A6
#define currentMeterPin A7

//EEPROM Adresses
#define voltToleranceEepromAdress 0
#define currentProtectionEepromAdress 5
#define batteryModeEepromAdress 7
#define questVoltEepromAdress 10
#define currentLimitEepromAdress 20

#define settingsAdress 0
#define versionAdress 250

//Charactiristics
#define MAX_VOLTAGE 24
#define VOLTAGE_SAMPLE_COUNT 10
#define CURRENT_SAMPLE_COUNT 10

//Error Codes
#define softwareMajorVersion 1
#define softwareMinorVersion 0
#define softwarePatchVersion 0

#define firmwareVersionErrorCode 11
#define versionNotSupportedCode 12
#define unknownFirmwareErrorCode 13
#define downGradedFirmwareErrorCode 14
#define programLoopFailureCode 15

#define overheatCode 21
#define temperatureSensorFailCode 22

#define voltageFluctuateErrorCode 31
#define voltageCalibrationErrorCode 32
#define currentCalculationFailErrorCode 33
#define mainVoltageLowErrorCode 34
#define mainVoltageCriticalErrorCode 35
#define mainVoltageHighErrorCode 36

//Display Colors
#define TFT_GRAY 0x7BEF

#endif