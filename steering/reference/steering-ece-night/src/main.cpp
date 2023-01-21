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
#include "steering_io.h"

CAL::CAL cal;

DataHolder ecu_data;
void setup() { pinMode(STEERING_CAN_OE, OUTPUT);
    digitalWrite(STEERING_CAN_OE, LOW);

    pinMode(STEERING_BUTTON_3, INPUT); // This one is plugged in on Jessie's setup

    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    TFT_init();

    // Initialize CAN BUS
	if (CANInit(CAN_500KBPS, 0, 2)) {
        Serial2.println("CAN BUS UP!");
    } else {
        Serial2.println("CAN INIT FAIL");
    }
    // Display splash logo
    TFT_splash();
    delay(3000);
}

uint8_t can_ch1 = 1;
CAL::CAN_msg_t can_msg;
bool button_prev = false;
bool button_curr = false;
void loop() {
    button_curr = !digitalRead(STEERING_BUTTON_3);

    if(CANMsgAvail(can_ch1)) {
        CANReceive(can_ch1, &can_msg);
        cal.updatePackage(can_msg);
        ecu_data.rpm = cal.returnVar(CAL::DATA_ECU::EngineRPM);
        ecu_data.coolant_temp = cal.returnVar(CAL::DATA_ECU::CoolantTemp);
        ecu_data.engine_temp = cal.returnVar(CAL::DATA_ECU::EngineTemp);
        ecu_data.speed = cal.returnVar(CAL::DATA_ECU::VehicleSpeed);
        ecu_data.tps = cal.returnVar(CAL::DATA_ECU::ThrottlePosition);
	}
    TFT_display();

    if (button_curr && !button_prev) {
        ecu_data.gear_pos++;
        if (ecu_data.gear_pos == 4) {
            ecu_data.gear_pos = 0;
        }
    }
    button_prev = button_curr;
    // if (!button_pressed && !digitalRead(STEERING_BUTTON_3)) {
    //     button_pressed = true;
    //     ecu_data.gear_pos++;
    //     if (ecu_data.gear_pos == 4) {
    //         ecu_data.gear_pos = 0;
    //     }
    // } else {
    //     button_pressed = false;
    // }
}
