
/**
 * @file interface_adc.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief FreeRTOS CAN bus Interface for QFSAE VCU
 * @version 0.1
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "hal/hal_adc.h"
#include "FreeRTOS.h"



/**
 * @brief OS ADC Setup Handler
 * 
 * This function should be called to init the ADC bus and handler interfaces
 * This function should not be called more than once
 * ALL CAN bus initialization should happen within this function
 * 
 * Called in main
 * 
 */
extern void os_adc_setup(void);

extern uint16_t adc_fetch(enum ADC_CHANNEL channel);
