/**
 * @file interface_inverter.h
 * @author Matt Dobaj
 * @brief FreeRTOS Inverter Interface for QFSAE VCU
 * @version 0.1
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "interfaces/interface_can.h"
#include "FreeRTOS.h"


/**
 * @brief OS CAN Setup Handler
 * 
 * This function should be called to init the connection to the inverters.
 * 
 * Called in main
 * 
 */
extern void os_inverter_setup(void);

extern void enable_motors(void);

extern void disable_motors(void);


