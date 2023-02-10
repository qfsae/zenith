/**
 * @file errors.h
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief Check all Engine Errors (for display)
 * @version 0.1
 * @date 2023-01-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ERRORS_H
#define ERRORS_H
#include <stdint.h>
    
    extern uint32_t _Time_Since_Last_CAN_msg;

    // Returns Error String For Fuel Pressure Errors
    const char * Err_Fuel_Pressure();

    // Returns Error String for Low Battery Voltage
    const char * Err_Battery();

    // Returns Error String for Stake CAN Data
    const char * Err_Stale_Data();

    // Returns Error String for BSPD
    const char * Err_BSPD();

    // Returns Error For Engine/Coolant Temp MisMatches
    const char * Err_Engine_Temp();

    // Returns Error String for ECU warning being true
    const char * Err_ECU_Warn();

#endif