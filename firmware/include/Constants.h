#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>
#include <SPI.h>

// --- PINOUT ---
namespace pins 
{
  constexpr byte BUZZER = 1;
  
  constexpr byte RIGHT_BUTTON = 0;
  constexpr byte UP_BUTTON = 2;
  constexpr byte DOWN_BUTTON = 3;
  constexpr byte LEFT_BUTTON = 4;
  constexpr byte FAN = 5;
  constexpr byte SCREEN_BACKLIGHT = 6;
  constexpr byte VOLTAGE_CONTROL = 9;
  constexpr byte CURRENT_CONTROL = 10;

  constexpr byte DTC = A0;
  constexpr byte POT = A1;
  constexpr byte TEMP_SENSOR = A2;
  constexpr byte VOLT_METER_MAIN = A3;
  constexpr byte POWER_CONTROL = A4;
  constexpr byte RELAY = A5;
  constexpr byte VOLT_METER = A6;
  constexpr byte CURRENT_METER = A7;

  constexpr byte TFT_CS = 7;
  constexpr byte TFT_DC = 8;
  constexpr byte TFT_MOSI = 11;
  constexpr byte TFT_CLK = 13;
}

namespace characteristics 
{
  constexpr float MAX_VOLTAGE = 24.0f;
  constexpr int VOLTAGE_SAMPLE_COUNT = 10;
  constexpr int CURRENT_SAMPLE_COUNT = 10;
}

// --- SOFTWARE VERSION ---
struct VersionInfo 
{
  byte major;
  byte minor;
  byte patch;

  bool operator<(const VersionInfo& other) const 
  {
    if (major != other.major) return major < other.major;
    if (minor != other.minor) return minor < other.minor;
    return patch < other.patch;
  }

  bool operator==(const VersionInfo& other) const 
  {
    return major == other.major && minor == other.minor && patch == other.patch;
  }
};

namespace system_version 
{
  constexpr VersionInfo FIRMWARE = {0, 7, 1};
  constexpr VersionInfo MIN_SUPPORTED = {0, 7, 0};
}

// --- ERROR CODES ---
enum class ErrorCode : byte 
{
  NONE                   =  0,
  FIRMWARE_VERSION_ERROR = 11,
  VERSION_NOT_SUPPORTED  = 12,
  DOWNGRADED_FIRMWARE    = 13,
  PROGRAM_LOOP_FAILURE   = 14,
  RAM_FAILURE            = 15,

  OVERHEAT               = 21,
  TEMP_SENSOR_FAIL       = 22,

  VOLTAGE_FLUCTUATE      = 31,
  VOLTAGE_CALIBRATION    = 32,
  CURRENT_CALC_FAIL      = 33,
  MAIN_VOLTAGE_LOW       = 34,
  MAIN_VOLTAGE_CRITICAL  = 35,
  MAIN_VOLTAGE_HIGH      = 36
};

#endif