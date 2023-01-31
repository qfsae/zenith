/**
 * @file main.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @author Logan Lim
 * @author Ethan Peterson (portfolio.petetech.net)
 * @brief Q22 QFSAE Steering Wheel Code
 * @version 1.2
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#include <SPI.h>

#include "EVE_target.h"
#include "EVE_commands.h"
#include "tft.h"
#include "cal.hpp"
#include "can.hpp"
#include "steering_io.h"
#include "EasyButton.h"

// Blink Rate of display elements
#define DISPLAY_BLINK_TIME 1000 //(ms)

#define UPSHIFT_DEBOUNCE_TIME 35
#define UPSHIFT_PULLUP_EN false // Pull up is provided on the physical PCB
#define UPSHIFT_ACTIVE_LOW true

EasyButton upshiftButton(STEERING_BUTTON_3, UPSHIFT_DEBOUNCE_TIME, UPSHIFT_PULLUP_EN, UPSHIFT_ACTIVE_LOW);
EasyButton downshiftButton(STEERING_BUTTON_4, UPSHIFT_DEBOUNCE_TIME, UPSHIFT_PULLUP_EN, UPSHIFT_ACTIVE_LOW);

CAL::CAL cal;

DataHolder ecu_data;

void upshift_handler() {
    Serial2.println("upshift!");
    if (ecu_data.gear_pos < 5) {
        ecu_data.gear_pos++;
    }
}

void downshift_handler() {
    Serial2.println("downshift!");
    if (ecu_data.gear_pos > 0) {
        ecu_data.gear_pos--;
    }
}

void setup() {
    ecu_data.gear_pos = 0;
    upshiftButton.begin(); 
    upshiftButton.onPressed(upshift_handler);

    downshiftButton.begin();
    downshiftButton.onPressed(downshift_handler);

    pinMode(STEERING_CAN_OE, OUTPUT);
    digitalWrite(STEERING_CAN_OE, LOW);

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
    delay(500);
}

uint8_t can_ch1 = 1;
CAN_msg_t can_msg;
uint32_t last_millis = 0;

void loop() {
    upshiftButton.read(); // call the polling updater in the library
    downshiftButton.read(); // call the polling updater in the library
    if(CANMsgAvail(can_ch1)) {
        CANReceive(can_ch1, &can_msg);
        cal.updatePackage(can_msg);
        ecu_data.rpm = cal.returnVar(CAL::DATA_ECU::EngineRPM);
        ecu_data.coolant_temp = cal.returnVar(CAL::DATA_ECU::CoolantTemp);
        ecu_data.engine_temp = cal.returnVar(CAL::DATA_ECU::EngineTemp);
        ecu_data.speed = cal.returnVar(CAL::DATA_ECU::VehicleSpeed);
        ecu_data.tps = cal.returnVar(CAL::DATA_ECU::ThrottlePosition);
        cal.returnVar(CAL::DATA_ECU::WarningSource, ecu_data.status);
        cal.returnVar(CAL::DATA_PDM::BatteryVoltage, ecu_data.batteryVoltage);
	}
    TFT_display();

    // display blinker
    if(millis() > (last_millis+DISPLAY_BLINK_TIME)){
        dswitch = !dswitch;
        last_millis = millis();
    }
}
