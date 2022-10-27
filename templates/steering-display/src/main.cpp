/*
@file    main.cpp
@brief   Main file for Steering Display Firmware
@version 1.0
@date    
@author  
*/

#include <Arduino.h>
#include <SPI.h>

#include "EVE_target.h"
#include "EVE_commands.h"
#include "tft.h"

void setup() {
    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

    TFT_init();

    // Display splash logo
    TFT_splash();
    delay(3000);
}

void loop() {   
    static uint32_t previous_millis = 0;
    static uint8_t display_delay = 0;
    uint32_t current_millis;
    current_millis = millis();
    
    if ((current_millis - previous_millis) > 3)
    {
        // Execute touch display code every ~5 ms
        previous_millis = current_millis;

        TFT_touch();

        display_delay++;
        if (display_delay > 3) { // Execute display refresh every ~20ms
            TFT_display();
        }
    }
}
