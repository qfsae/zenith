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


#define STEERING_BUTTON_3 PC6
#define STEERING_BUTTON_4 PB15


void setup() {
    pinMode(STEERING_BUTTON_3, INPUT_PULLUP);
    pinMode(STEERING_BUTTON_4, INPUT_PULLUP);

    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    TFT_init();

    gear_position = 0;

    // // Display splash logo
    TFT_splash();
    delay(3000);
}

void loop() {   
    // read input pins
    uint16_t last_button_state_3, last_button_state_4;
    uint16_t steering_3 = digitalRead(STEERING_BUTTON_3);   // read the input pin
    uint16_t steering_4 = digitalRead(STEERING_BUTTON_4);

    // compare the buttonState to its previous state
    if (steering_3 != last_button_state_3) {
        if (steering_3 == LOW) {
            gear_position++;
        }
        delay(50); // delay in case a
    }
    last_button_state_3 = steering_3;

    // compare the buttonState to its previous state
    if (steering_4 != last_button_state_4) {
        if (steering_4 == LOW) {
            gear_position--;
        }
        delay(50);
    }
    last_button_state_4 = steering_4;

    TFT_display();

}
