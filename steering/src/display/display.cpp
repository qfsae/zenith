/**
 * @file display.cpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "display/display.hpp"
#include "SPI.h"
#include "EVE.hpp"
#include "display/tft_data.hpp"
#include "display/consts.hh"
#include "main.hpp"

// Memory-map defines
#define MEM_LOGO 0x00000000 // Start-address of logo
#define CENTER_N 0x0000f000 // start-address of n_center, needs 48672 bytes
#define CENTER_1 0x0001AE20 // start address of center_1, needs 48672 bytes
#define CENTER_2 0x00026C40 // start address of center_2, needs 48672 bytes
#define CENTER_3 0x00032A60 // start address of center_3, needs 48672 bytes
#define CENTER_4 0x0003E880 // start address of center_4, needs 48672 bytes
#define CENTER_5 0x0004A6A0 // start address of center_5, needs 48672 bytes


String Display::checkErrors(){
    String ret = "";
    if((millis()-lastCAN) > 3000){
        ret += " Stale CAN Data ";
    }
    if(CAN_Init_Error == true){
        ret += " CAN Init Error ";
    }
    if(cal.returnVar(CAL::DATA_ECU::WarningSource)){
        ret += " ECU WARNING ";
    }
    if(cal.returnVar_os(CAL::DATA_PDM::StarterStatus).Active == false && cal.returnVar_f(CAL::DATA_PDM::BatteryVoltage) < 12.5){
        ret += " REPLACE BATTERY ";
    }
    else if(cal.returnVar_f(CAL::DATA_PDM::BatteryVoltage) < 13){
        ret += " Battery Low: ";
        ret += FLOAT_STRING(cal.returnVar_f(CAL::DATA_PDM::BatteryVoltage)).c_str();
        ret += "V ";
    };
    if(cal.returnVar_os(CAL::DATA_PDM::StarterStatus).Fault == true){
        ret += " Starter Fault ";
    }
    // if(cal.returnVar_os(CAL::DATA_PDM::FuelPumpStatus).Active == true && cal.returnVar_f(CAL::DATA_PDM::FuelPumpCurrent) < 0.5){
    //     ret += " Fuel Pump Disconnect ";
    // }
    // if(cal.returnVar_os(CAL::DATA_PDM::FuelPumpStatus).Fault){
    //     ret += " Fuel Pump Fault ";
    // }
    return ret;
}

void Display::setup(){

    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);
    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

    if(E_OK == EVE_init())
    {
        tft_active = 1;

        EVE_memWrite8(REG_PWM_DUTY, 0x10);  // Set up backlight, range is from 0 = off to 0x80 = max
        EVE_memWrite16(REG_PWM_HZ, 0x2710); // Set backlight hz to maximum (10000hz)

        /* LOAD IMAGES INTO MEMORY HERE */
        EVE_cmd_inflate(MEM_LOGO, logo, sizeof(logo)); // Load logo into gfx-memory and de-compress it
        EVE_cmd_inflate(CENTER_N, center_n, sizeof(center_n)); // Load logo into gfx-memory and de-compress it
        EVE_cmd_inflate(CENTER_1, center_1, sizeof(center_1));
        EVE_cmd_inflate(CENTER_2, center_2, sizeof(center_2));
        EVE_cmd_inflate(CENTER_3, center_3, sizeof(center_3));
        EVE_cmd_inflate(CENTER_4, center_4, sizeof(center_4));
        EVE_cmd_inflate(CENTER_5, center_5, sizeof(center_5));
        /* LOAD IMAGES INTO MEMORY HERE */
    }
}

void Display::display(Screens screen){

    if((millis()-millis_last) > 1000){
        clock = !clock;
        millis_last = millis();
    }

    switch (screen)
    {

    case Display::Screens::Splash:
        displaySplash();
        break;
    case Display::Screens::WheelDiagnostic:
        displayWheelDiagnostic();
    break;
    case Display::Screens::Main:
        displayMain();
        break;

    default:
        break;

    }
}

void Display::updateCAN(){
    lastCAN = millis();
}