/**
 * @file interface_adc.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief FreeRTOS ADC Interface for QFSAE VCU
 * @version 0.1
 * @date 2024-02-24
 * 
 * @copyright Copyright (c) 2024
 * 
 * This implementation is based off of an interrupt approach to reading the ADC.
 * TODO: Add DMA support for ADC
 * 
 */

#include "interfaces/interface_adc.h"
#include "hal/hal_gpio.h"
#include "interfaces/interface_sysError.h"
#include "nvicConfig.h"
#include "interrupts.h"

uint16_t ADC_READINGS[ADC_CHANNEL_MAX];
uint8_t  ADC_READING_CURRENT = 0;


void os_adc_setup(void) {

    // Zero out ADC Readings
    for (int i = 0; i < ADC_CHANNEL_MAX; i++)
    {
        ADC_READINGS[i] = 0;
    }
    

    // Reduce the ADC Clock to APB2/8
    SET_BIT(ADC123_COMMON->CCR, 3 << 16);

    // Configure Analog GPIO Pins
    gpio_set_mode(PIN_A5Vin_1, GPIO_MODE_ANALOG);
    gpio_set_mode(PIN_A5Vin_2, GPIO_MODE_ANALOG);

    // Setup ADC1
    hal_adc_init(ADC1, ADC_RESOLUTION_12_BIT);
    // Setup Channel Sequence
    hal_adc_configChannel(ADC1, 4, ADC_CYCLES_480, ADC_SQ1);
    hal_adc_configChannel(ADC1, 5, ADC_CYCLES_480, ADC_SQ2);
    // Set Sequence Length
    hal_adc_set_sequence_len(ADC1, 2);

    // Setup Interrupt
    SET_BIT(ADC1->CR1, ADC_CR1_EOCIE);
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, NVIC_Priority_MIN);

    spin(9999999UL); // Wait for ADC to stabilize

    // Enable ADC and Start Conversions
    hal_adc_startConversions(ADC1);
}

void vTask_ADCMonitor(void *param) {
    (void)(param); // Cast unused variable to void
    vTaskDelay(5);
    printf("ADC Monitor Task Running\n");
    for(;;) {
        // Print ADC Readings
        if(READ_BIT(ADC1->SR, ADC_SR_OVR)) sysError_report(ADC_OVERFLOW);
        if(sysError_check(ADC_OVERFLOW) == true){
            // Attempt to clear the overflow
            hal_adc_stopConversions(ADC1);
            vTaskDelay(5);
            CLEAR_BIT(ADC1->SR, ADC_SR_OVR);
            vTaskDelay(5);
            hal_adc_startConversions(ADC1);
            if(READ_BIT(ADC1->SR, ADC_SR_STRT)){
                sysError_clear(ADC_OVERFLOW);
            }
        }
        // Trigger a new round of conversions
        SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
        vTaskDelay(50);
    }
}

void ADC_IRQHandler(void){
    if(READ_BIT(ADC1->SR, ADC_SR_EOC)){
        ADC_READINGS[ADC_READING_CURRENT] = hal_adc_readConversion(ADC1);
        ADC_READING_CURRENT++;
        if(ADC_READING_CURRENT == ADC_CHANNEL_MAX) ADC_READING_CURRENT = 0;
    }
}


extern double adc_fetch(enum ADC_CHANNEL channel){
    if(channel > ADC_CHANNEL_MAX) return 0.0;
    // TODO: add scaling for 12v/5v
    return ADC_READINGS[channel]*3.3/4096.0;
}
