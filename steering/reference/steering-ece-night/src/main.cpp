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
#include "cal.hpp"
#include "can.hpp"


#define STEERING_BUTTON_3 PC6
#define STEERING_BUTTON_4 PB15

void setup() { 
    pinMode(STEERING_BUTTON_3, INPUT_PULLUP);
    pinMode(STEERING_BUTTON_4, INPUT_PULLUP); // This one is plugged in on Jessie's setup

    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    TFT_init();

	if (CANInit(CAN_500KBPS, 0, 2)) {
        Serial2.println("CAN BUS UP!");
    } else {
        Serial2.println("CAN INIT FAIL");
    }
    gear_position = 0;
    // Initialize CAN BUS
    // Display splash logo
    TFT_splash();
    delay(3000);
}

void loop() {
    TFT_display();
}
