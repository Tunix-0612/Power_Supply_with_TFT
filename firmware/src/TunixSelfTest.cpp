#include "TunixSelfTest.h"

#include "Constants.h"
#include "TempController.h"
#include "OutputManager.h"
#include "TunixErrorManager.h"

TunixSelfTest::TunixSelfTest() { }

// --- Arduino check free RAM, return by byt
int TunixSelfTest::getFreeRAM() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

// --- Arduino RAM test
bool TunixSelfTest::runRAMTest() 
{
  // RAM start and end adresses for ATMEGA328P
  // Start Adress 0x0100 (Internal SRAM)
  // End Adress RAMEND (0x08FF)
  noInterrupts(); // Disable Interrupts
  extern int __heap_start, *__brkval;
  
  int* free_memory_start = (__brkval == 0 ? &__heap_start : __brkval);

  int* free_memory_end = (int*)((uintptr_t)RAMEND - 32); // Preserve the upper 32 stack

  // Scan the Dynamical RAM area that has been defined
  for (int* ptr = free_memory_start; ptr < free_memory_end; ptr++) 
  {
    int originalValue = *ptr; // Back-up the original value

    // Test 1: Write 0x5555 and read (01010101 01010101)
    *ptr = 0x5555;
    if (*ptr != 0x5555) { interrupts(); return true; } // Faulty RAM

    // Test 2: Write 0xAAAA and read (10101010 10101010)
    *ptr = (int)0xAAAA;
    if (*ptr != (int)0xAAAA) { interrupts(); return true; } // Faulty RAM
    
    *ptr = originalValue; // Load the original value back
  }
  interrupts();
  return false; // All Registers are Healthy
}

ErrorCode TunixSelfTest::selfTest()
{
	if(runRAMTest()) return ErrorCode::RAM_FAILURE;
	
  int temperatureStatus = temperature.tempControl();
  float voltCalculated = output.readVoltage();
  float voltMainCalculated = output.readMainVoltage();
  float currentCalculated = output.readCurrent();
  if(temperatureStatus == 1 && temperatureStatus == 0) errorManager.errorHandler(ErrorCode::TEMP_SENSOR_FAIL);
  if(temperatureStatus >= 85) errorManager.errorHandler(ErrorCode::OVERHEAT);
  if(voltCalculated > 1.00) errorManager.errorHandler(ErrorCode::VOLTAGE_FLUCTUATE);
  if(voltMainCalculated < 7.00) errorManager.errorHandler(ErrorCode::MAIN_VOLTAGE_LOW);
  if(currentCalculated > 0.05) errorManager.errorHandler(ErrorCode::CURRENT_CALC_FAIL);

	return ErrorCode::NONE;
}
