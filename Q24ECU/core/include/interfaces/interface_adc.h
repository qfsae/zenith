
/**
 * @file interface_adc.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief FreeRTOS ADC Interface for QFSAE VCU
 * @version 0.1
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "float.h"
#include "hal/hal_adc.h"
#include "FreeRTOS.h"

#define ADC_5V_RATIO (double)(1.0)
#define ADC_12V_RATIO (double)(1.0)

/**
 * @brief ADC Channels (According to the VCU)
 * 
 */
enum ADC_CHANNEL {
    ADC_CHANNEL_5V1,
    ADC_CHANNEL_5V2,
    //ADC_CHANNEL_5V3,
    //ADC_CHANNEL_5V4,
    //ADC_CHANNEL_12V1,
    //ADC_CHANNEL_12V2,
    //ADC_CHANNEL_12V3,
    //ADC_CHANNEL_12V4,

    // Leave last
    ADC_CHANNEL_MAX
};

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

/**
 * @brief Fetch the last recorded voltage of an ADC channel
 * 
 * @param channel the channel to draw from
 * @returns the voltage of the channel
 */
extern double adc_fetch(enum ADC_CHANNEL channel);
