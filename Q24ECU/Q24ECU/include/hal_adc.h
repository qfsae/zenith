#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stm32f446xx.h>
#include "hal_gpio.h"

static inline void adc_init(ADC_TypeDef *adc){

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
    adc->CR1 &= ~(ADC_CR1_DISCEN);
    // set number of conversions
    adc->SQR1 &= ~(ADC_SQR1_L);
    adc->SQR1 |= (1UL << 20U);
    // disable ADC DMA continuous request
    adc->CR2 &= ~(ADC_CR2_DDS);
    // end of conversion selection
    adc->CR2 &= ~(ADC_CR2_EOCS);
    adc->CR2 |= (0x00000001U << 10U);
}

static inline void adc_configChannel(ADC_TypeDef *adc, uint8_t ch, uint8_t rank){
    if(ch > 9){
        adc->SMPR1 &= ~(0x7 << (3U * (uint32_t)(ch-10U)));
        adc->SMPR1 |= (0x00000000U << (3U * (uint32_t)(ch-10U)));
    }
    else{
        adc->SMPR2 &= ~(0x7 << (3U * (uint32_t)(ch)));
        adc->SMPR2 |= (0x00000000U << (3U * (uint32_t)(ch)));
    }
    if(rank < 7U){
        adc->SQR3 &= ~((((uint32_t)((uint16_t)(0x1FUL))) << (5U * ((rank) - 1U))));
        adc->SQR3 |= ((((uint32_t)((uint16_t)(ch))) << (5U * ((rank) - 1U))));
    }
    else if(rank < 13U){
        adc->SQR2 &= ~((((uint32_t)((uint16_t)(0x1FUL))) << (5U * ((rank) - 7U))));
        adc->SQR2 |= ((((uint32_t)((uint16_t)(ch))) << (5U * ((rank) - 7U))));
    }
    else{
        adc->SQR1 &= ~((((uint32_t)((uint16_t)(0x1FUL))) << (5U * ((rank) - 13U))));
        adc->SQR1 |= ((((uint32_t)((uint16_t)(ch))) << (5U * ((rank) - 13U))));
    }
}

static inline void adc_start(ADC_TypeDef *adc){
    if((adc->CR2 & ADC_CR2_ADON) != ADC_CR2_ADON){
        adc->CR2 |= ADC_CR2_ADON;
        volatile uint32_t counter = 0U;
        counter = 3U * (SystemCoreClock/1000000U);
        // wait for adc to stabilize
        while (counter != 0U) counter--;
    }
    if((adc->CR2 & ADC_CR2_ADON) == ADC_CR2_ADON){
        adc->CR2 |= (uint32_t)ADC_CR2_SWSTART;
    }    
}

static inline void adc_stop(ADC_TypeDef *adc){
    adc->CR2 &= ~(ADC_CR2_SWSTART);
}

static inline uint16_t adc_poll(ADC_TypeDef *adc, uint8_t adcCh){
    adc_configChannel(adc, adcCh, 1);
    adc_start(adc);
    while((adc->SR & ADC_SR_EOC) != ADC_SR_EOC);
    return adc->DR;
}