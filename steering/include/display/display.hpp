/**
 * @file display.hpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief TFT display class
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include "stdint.h"
#include <Arduino.h>

class Display{
private:

    // FSAE Logo Splash Screen
    void displaySplash();
    // Main Driver Display
    void displayMain();
    // Wheel Diagnostic Screen
    void displayWheelDiagnostic();

    // TFT Active
    uint16_t tft_active = 0;

    // TFT Clock (for blinking)
    bool clock = false;
    uint32_t millis_last;

    // coolant bar colors
    uint32_t coolantColors[5] = {0x00D1FF, 0xe6cc00, 0xe6b400, 0xe69b00, 0xe47200};

    String checkErrors();

    uint32_t lastCAN;

public:

    // Engine Gear
    uint8_t gear = 0;
    // CAN initialization Error
    bool CAN_Init_Error = false;

    // List of Screens
    enum Screens{
        Splash,
        WheelDiagnostic,
        Main
    };
    // Important: Splash must be first and Main must be last in enum

    // Run Display Setup
    void setup();

    // TFT refresh
    void display(Screens screen);

    void updateCAN();
    
};