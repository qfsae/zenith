
/**
 * @file interface_adc.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief FreeRTOS ADC Interface for QFSAE VCU
 * @version 1.1
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "float.h"
#include "inttypes.h"
#include "hal/hal_adc.h"
#include "hal/hal_dma.h"
#include "hal/hal_gpio.h"
#include "FreeRTOS.h"
#include "pins.h"

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
 * @brief ADC Initialization Handler
 * 
 * This function will initialize the ADC and DMA interfaces.
 * This function should not be called more than once
 * 
 */
extern void adc_init(void);

/**
 * @brief Read the last recorded voltage of an ADC channel
 * 
 * @param channel the channel to read
 * @returns the voltage of the channel
 */
extern double adc_read(enum ADC_CHANNEL channel);
