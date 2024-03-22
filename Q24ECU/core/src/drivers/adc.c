/**
 * @file adc.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Source File for ADC Driver
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/adc.h"
#include "hal/hal_adc.h"
#include "hal/hal_dma.h"
#include "hal/hal_gpio.h"
#include "hal/hal_clock.h"

uint16_t ADC_READINGS[ADC_CHANNEL_MAX];

void adc_init(void){
    
    // Zero out ADC Readings
    for(int i = 0; i < ADC_CHANNEL_MAX; i++){
        ADC_READINGS[i] = 0;
    }

    // Reduce the ADC Clock to APB2/8
    SET_BIT(ADC123_COMMON->CCR, 3 << 16);

    // Configure Analog GPIO Pins
    gpio_set_mode(PIN_A5Vin_1, GPIO_MODE_ANALOG);
    gpio_set_mode(PIN_A5Vin_2, GPIO_MODE_ANALOG);

    // Setup DMA for ADC1
    hal_dma_init(DMA2, DMA2_Stream0, DMA_CHANNEL_0, DMA_PRIORITY_LOW, DMA_MEM_SIZE_16, &(ADC1->DR), ADC_READINGS, ADC_CHANNEL_MAX);
    // Setup ADC1
    hal_adc_init(ADC1, ADC_RESOLUTION_12_BIT);
    // Setup Channel Sequence
    hal_adc_configChannel(ADC1, PIN_A5Vin_1_ADCCH, ADC_CYCLES_480, ADC_SQ1);
    hal_adc_configChannel(ADC1, PIN_A5Vin_2_ADCCH, ADC_CYCLES_480, ADC_SQ2);
    // Set Sequence Length
    hal_adc_set_sequence_len(ADC1, ADC_CHANNEL_MAX);

    spin(9999999UL); // Wait for ADC to stabilize

    // Enable the DMA Stream
    hal_dma_start(DMA2_Stream0);
    // Enable ADC and Start Conversions
    hal_adc_startConversions(ADC1);
}

double adc_read(enum ADC_CHANNEL channel){
    // Return obviously bad data if ADC is not enabled
    if(!READ_BIT(ADC1->CR2, ADC_CR2_ADON)) return 123.456;
    // Return 0 if ADC has encountered an overrun
    if(READ_BIT(ADC1->SR, ADC_SR_OVR)) return 0.0;
    // Return 0 if channel is out of range
    if(channel > ADC_CHANNEL_MAX) return 0.0;
    // TODO: add scaling for 12v/5v
    return ADC_READINGS[channel]*3.3/4096.0;
}

