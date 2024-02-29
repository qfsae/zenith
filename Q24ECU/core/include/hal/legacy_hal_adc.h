/**
 * @file legacy_hal_adc.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 * Legacy Version of the ADC HAL
 * 
 */
#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "hal_clock.h"
#include <stm32f446xx.h>

static inline void legacy_hal_adc_init(ADC_TypeDef *adc){

    // enable the selected adc interface
    if(adc==ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    if(adc==ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
    if(adc==ADC3) RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;


    // Set the ADC clock prescaler
    ADC123_COMMON->CCR &= ~(ADC_CCR_ADCPRE);
    ADC123_COMMON->CCR |= 0x00000000U; 
    // Set the ADC Resolution
    adc->CR1 &= ~(ADC_CR1_SCAN);
    adc->CR1 |= (0 << 8U); 
    // Set the ADC data alignment (bitwise right align)
    adc->CR2 &= ~(ADC_CR2_ALIGN);
    adc->CR2 |= 0x00000000U; 
    //reset the ADC trigger to software
    adc->CR2 &= ~(ADC_CR2_EXTSEL);
    adc->CR2 &= ~(ADC_CR2_EXTEN);
    // Disable continuous conversion mode
    adc->CR2 &= ~(ADC_CR2_CONT);
    adc->CR1 &= ~(ADC_CR1_DISCEN); //disables discont mode
    // set number of conversions
    adc->SQR1 &= ~(ADC_SQR1_L);
    adc->SQR1 |= (1UL << 20U);
    // disable ADC DMA continuous request
    adc->CR2 &= ~(ADC_CR2_DDS);
    // end of conversion selection
    adc->CR2 |= (ADC_CR2_EOCS);
}

static inline void legacy_hal_adc_configChannel(ADC_TypeDef *adc, uint8_t ch, uint8_t rank){
    if(ch > 9){
        adc->SMPR1 &= ~(0x7UL << (3U * ((uint32_t)(ch-10U))));
        adc->SMPR1 |= (0x00000000U << (3U * (uint32_t)(ch-10U)));
    }
    else{
        adc->SMPR2 &= ~(0x7UL << (3U * ((uint32_t)(ch))));
        adc->SMPR2 |= (0x00000000U << (3U * (uint32_t)(ch)));
    }
    if(rank < 7U){
        adc->SQR3 &= ~(uint32_t)(0x1FU << (5U * ((rank) - 1U)));
        adc->SQR3 |= (uint32_t)(ch << (5U * ((rank) - 1U)));
    }
    else if(rank < 13U){
        adc->SQR2 &= ~(uint32_t)(0x1FU << (5U * ((rank) - 7U)));
        adc->SQR2 |= (uint32_t)(ch << (5U * ((rank) - 7U)));
    }
    else{
        adc->SQR1 &= ~(uint32_t)(0x1FU << (5U * ((rank) - 13U)));
        adc->SQR1 |= (uint32_t)(ch << (5U * ((rank) - 13U)));
    }
}

static inline void legacy_hal_adc_start(ADC_TypeDef *adc){
    if(!(adc->CR2 & ADC_CR2_ADON)){
        adc->CR2 |= ADC_CR2_ADON;
        volatile uint32_t counter = 0U;
        counter = 3U * (SYS_FREQUENCY/1000000U);
        // wait for adc to stabilize
        while (counter != 0U) counter--;
    }
    adc->CR2 |= ADC_CR2_SWSTART;    
}

static inline void legacy_hal_adc_stop(ADC_TypeDef *adc){
    adc->CR2 &= ~(ADC_CR2_SWSTART);
}

static inline uint16_t legacy_hal_adc_poll(ADC_TypeDef *adc, uint8_t adcCh){
    legacy_hal_adc_configChannel(adc, adcCh, 1);
    legacy_hal_adc_start(adc);
    while(!(adc->SR & ADC_SR_EOC));
    return (uint16_t)(adc->DR);

}

