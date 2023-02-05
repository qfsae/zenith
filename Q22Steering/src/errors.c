/**
 * @file errors.c
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief Check All Engine Errors (for display)
 * @version 0.1
 * @date 2023-01-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "errors.h"

uint32_t _Time_Since_Last_CAN_msg = 0;

// Returns Error String For Fuel Pressure Errors
const char * Err_Fuel_Pressure(){
    if(1){
        return 0;
    }
    else{
        return "Fuel Pressure Fault";
    }
}

// Returns Error String for Low Battery Voltage
const char * Err_Battery(){
    if(1){
        return 0;
    }
    else{
        return "Battery Fault";
    }
}

// Returns Error String for Stake CAN Data
const char * Err_Stale_Data(){
    if(_Time_Since_Last_CAN_msg > 4000){
        return "Stale CAN DATA";
    }
    else{
        return 0;
    }
}

// Returns Error String for BSPD
const char * Err_BSPD(){
    if(1){
        return 0;
    }
    else{
        return "BSPD FAULT";
    }
}

// Returns Error For Engine/Coolant Temp MisMatches
const char * Err_Engine_Temp(){
    if(1){
        return 0;
    }
    else{
        return "Engine Temp Fault";
    }
}

// Returns Error String for ECU warning being true
const char * Err_ECU_Warn(){
    if(1){
        return 0;
    }
    else{
        return "General ECU Warning";
    }
}