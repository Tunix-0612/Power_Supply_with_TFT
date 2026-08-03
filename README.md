# Power Supply TFT Firmware (Tunix Core V3)

[![PlatformIO](https://img.shields.io/badge/PlatformIO-Build-orange?style=flat-square&logo=platformio)](https://platformio.org/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-blue?style=flat-square&logo=arduino)](https://www.arduino.cc/)
[![MCU](https://img.shields.io/badge/MCU-ATmega328P-green?style=flat-square)](https://www.microchip.com/)

A modular, highly optimized, and object-oriented C++ firmware for TL494-based programmable lab power supplies. Built on top of Tunix Core Modules V3, this firmware provides real-time voltage/current metering, feedback control, digital safety protections, custom UI rendering on a 128x128 ST7735 TFT display, and non-volatile configuration management.

---

## Key Features

* **TL494 PWM Controller Integration:** Designed to interface with the TL494 PWM control IC, providing microcontroller-guided feedback loop adjustments, output enable logic, and hardware protection hooks.
* **Tunix Core Modules V3 Engine:** Event-driven, non-blocking subsystem architecture optimized for 8-bit AVR microcontrollers.
* **Hardware Abstraction Layer (HAL):** Encapsulated DisplayDriver isolates Adafruit_ST7735 primitives, eliminating direct display driver leakage across upper application logic.
* **Fast & Optimized ADC Routines:** Non-blocking sampling algorithms eliminate delay() execution, maximizing Over-Current Protection (OCP) response time.
* **Strict C++ Standards:** Built using modern Object-Oriented Programming (OOP) principles, zero global scope pollution, and type-safe VersionInfo structure management.
* **Smart Operating Modes:** Supports Basic and Advanced UI screens, Voltage and Current Control modes and output relay/MOSFET control.
* **Input Management:** Event-driven button handling via InputManager supporting debounced multi-button states.

---

## System Architecture & Directory Layout

The codebase follows the standard PlatformIO directory structure, isolating header definitions from implementation files:

```text
Power_Supply_TFT/
├── firmware/
│   ├── include/                # Header files (.h)
│   │   ├── Constants.h         # System configurations, pins & Error codes
│   │   ├── DisplayDriver.h     # Encapsulated TFT Driver & UI primitives
│   │   ├── OutputManager.h     # ADC Reading & Voltage/Current calculations
│   │   ├── InputManager.h      # Event-driven Button Engine
│   │   ├── Menu.h              # Menu system and navigation routines
│   │   └── ...
│   └── src/                    # Source implementation files (.cpp)
│       ├── main.cpp            # Application Entry Point & Instantiation
│       ├── DisplayDriver.cpp
│       ├── OutputManager.cpp
│       └── ...
└── platformio.ini              # Project build flags and library dependencies
