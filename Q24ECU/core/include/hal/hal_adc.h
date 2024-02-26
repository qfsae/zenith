/**
 * @file hal_adc.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * ADC Hardware Abstraction Layer designed to be used in tandem with DMA HAL.
 *
 * For Simple Polling ADC usage, the Legacy HAL is also provided in another file.
 *
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "stm32f4xx.h"
#include "errors.h"
#include "hal_clock.h"

// ADC Sampling Resolution (Should always be 12bit)
enum ADC_RESOLUTION {
    ADC_RESOLUTION_12_BIT = 0x0,
    ADC_RESOLUTION_10_BIT = 0x1,
    ADC_RESOLUTION_8_BIT  = 0x2,
    ADC_RESOLUTION_6_BIT  = 0x3
};

/** ADC Sampling Time (Clock Cycles)
 * More samples = More Accurate
 * Less samples = Shorter Delay between conversions
 */
enum ADC_SAMPLE_TIME {
    ADC_CYCLES_3,
    ADC_CYCLES_15,
    ADC_CYCLES_28,
    ADC_CYCLES_56,
    ADC_CYCLES_84,
    ADC_CYCLES_112,
    ADC_CYCLES_144,
    ADC_CYCLES_480
};

// Default ADC Sampling Time (MUST be 480 on VCU V1)
#define ADC_SAMPLE_TIME_DEFAULT ADC_CYCLES_480


/**
 * @brief ADC Sequence Positions
 *
 * There can be a maximum of 16 channels added to an ADC Sequence
 */
enum ADC_SEQUENCE {
    ADC_SQ1 = 1,
    ADC_SQ2,
    ADC_SQ3,
    ADC_SQ4,
    ADC_SQ5,
    ADC_SQ6,
    ADC_SQ7,
    ADC_SQ8,
    ADC_SQ9,
    ADC_SQ10,
    ADC_SQ11,
    ADC_SQ12,
    ADC_SQ13,
    ADC_SQ14,
    ADC_SQ15,
    ADC_SQ16
};

/**
 * @brief Initialize ADC Hardware for use with DMA + Continuous Conversions of Sequenced Channels
 * 
 * @param adc The ADC to Initialize
 * @param resolution The resolution of the ADC (in bits)
 * @returns An Error or SYS_OK 
 */
static inline enum SYS_ERROR hal_adc_init(ADC_TypeDef *adc, enum ADC_RESOLUTION resolution){
    if(adc == ADC1) SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
    if(adc == ADC2) SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);
    if(adc == ADC3) SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);

    // Disable ADC Overrun Interrupt
    CLEAR_BIT(adc->CR1, ADC_CR1_OVRIE);
    // Set ADC Resolution
    SET_BIT(adc->CR1, (uint32_t)(resolution << ADC_CR1_RES_Pos));
    // Disable Analog watchdog
    CLEAR_BIT(adc->CR1, ADC_CR1_AWDEN); // Regular channels
    CLEAR_BIT(adc->CR1, ADC_CR1_AWDEN); // Injected channels
    // Reset Discontinuous Channel selection
    CLEAR_BIT(adc->CR1, ADC_CR1_DISCNUM);
    CLEAR_BIT(adc->CR1, ADC_CR1_JDISCEN);
    // Disable Discontinuous mode on regular channels
    CLEAR_BIT(adc->CR1, ADC_CR1_DISCEN);
    // Disable automatic injected group conversion
    CLEAR_BIT(adc->CR1, ADC_CR1_JAUTO);
    // Set scan mode (Forces the ADC to loop through the Sequence)
    SET_BIT(adc->CR1, ADC_CR1_SCAN);
    // Disable End of Conversion Interrupt for Injected Channels
    CLEAR_BIT(adc->CR1, ADC_CR1_JEOCIE);
    // Disable the Analog Watchdog Interrupt
    CLEAR_BIT(adc->CR1, ADC_CR1_AWDIE);
    // Disable End of Conversion Interrupt
    CLEAR_BIT(adc->CR1, ADC_CR1_EOCIE);
    
    // Reset Software Start bit for regular channels
    CLEAR_BIT(adc->CR2, ADC_CR2_SWSTART);
    // Disable External trigger for regular channels
    CLEAR_BIT(adc->CR2, ADC_CR2_EXTEN);
    // Reset Software start for injected channels
    CLEAR_BIT(adc->CR2, ADC_CR2_JSWSTART);
    // Disable External trigger for injected channels
    CLEAR_BIT(adc->CR2, ADC_CR2_JEXTEN);
    // Set data alignment (Right Align)
    CLEAR_BIT(adc->CR2, ADC_CR2_ALIGN);
    // End of Conversion Bit enable
    SET_BIT(adc->CR2, ADC_CR2_EOCS);
    // Enable DMA + Repeat DMA Requests
    SET_BIT(adc->CR2, ADC_CR2_DMA);
    SET_BIT(adc->CR2, ADC_CR2_DDS);
    // Set Continuous conversion (ADC Loops back to first conversion after completing the Sequence)
    SET_BIT(adc->CR2, ADC_CR2_CONT);

    // Leave ADC Off
    CLEAR_BIT(adc->CR2, ADC_CR2_ADON);

    return SYS_OK;
}

/**
 * @brief Enable the ADC (ADON)
 * 
 * @param adc The ADC to Enable
 */
static inline void hal_adc_enable(ADC_TypeDef *adc){
    if(!READ_BIT(adc->CR2, ADC_CR2_ADON))
        SET_BIT(adc->CR2, ADC_CR2_ADON);
}

/**
 * @brief Disable an ADC
 * 
 * @param adc The ADC to Disable
 */
static inline void hal_adc_disable(ADC_TypeDef *adc){
    if(READ_BIT(adc->CR2, ADC_CR2_ADON))
        CLEAR_BIT(adc->CR2, ADC_CR2_ADON);
}

/**
 * @brief Start ADC Conversions
 * 
 * @param adc The ADC to Start
 */
static inline void hal_adc_startConversions(ADC_TypeDef *adc){
    hal_adc_enable(adc);
    for (unsigned int i = 0; i < (3U * (SYS_FREQUENCY/1000000U)); i++) __asm__("nop");
    SET_BIT(adc->CR2, ADC_CR2_SWSTART);
}

/**
 * @brief Stop ADC Conversions
 * 
 * @param adc The ADC to Stop
 */
static inline void hal_adc_stopConversions(ADC_TypeDef *adc){
    CLEAR_BIT(adc->CR2, ADC_CR2_SWSTART);
}

/**
 * @brief Configure an ADC Channel
 * 
 * @param adc The ADC to Configure
 * @param channel The Channel to Configure
 * @param cycles The Sample Time
 * @param rank The Sequence Rank
 */
static inline void hal_adc_configChannel(ADC_TypeDef *adc, uint8_t channel, enum ADC_SAMPLE_TIME cycles, enum ADC_SEQUENCE rank){
    (void)cycles;
    // Setup Channel Sample Time
    if(channel > 9) { // High Channel Register
                      // Reset Sample Time
        CLEAR_BIT(adc->SMPR1, (uint32_t)(0x7UL << (3U * (uint32_t)(channel-10U))));
        // Set Sample Time
        SET_BIT(adc->SMPR1, (uint32_t)(cycles << (3U * (uint32_t)(channel-10U))));
    }
    else { // Low Channel Register
        CLEAR_BIT(adc->SMPR2, (uint32_t)(0x7UL << (3U * (uint32_t)(channel))));
        SET_BIT(adc->SMPR2, (uint32_t)(cycles << (3U * (uint32_t)(channel))));
    }

    // Add Channel to the Sequence Register
    if(rank < 7U) {
        CLEAR_BIT(adc->SQR3, (uint32_t)(0x1FU << (5U * ((rank) - 1U))));
        SET_BIT(adc->SQR3, (uint32_t)(channel << (5U * ((rank) - 1U))));
    }
    else if(rank < 13U) {
        CLEAR_BIT(adc->SQR2, (uint32_t)(0x1FU << (5U * ((rank) - 7U))));
        SET_BIT(adc->SQR2, (uint32_t)(channel << (5U * ((rank) - 7U))));
    }
    else{
        CLEAR_BIT(adc->SQR1, (uint32_t)(0x1FU << (5U * ((rank) - 13U))));
        SET_BIT(adc->SQR1, (uint32_t)(channel << (5U * ((rank) - 13U))));
    }
}

/**
 * @brief Set the ADC Sequence Length
 * 
 * @param adc The ADC to Configure
 * @param len The Length of the Sequence
 */
static inline void hal_adc_set_sequence_len(ADC_TypeDef *adc, uint8_t len){
    CLEAR_BIT(adc->SQR1, ADC_SQR1_L);
    SET_BIT(adc->SQR1, (uint32_t)((len - 1) << ADC_SQR1_L_Pos));
}

/**
 * @brief Read the Last Conversion
 * 
 * @param adc The ADC to Read
 * @returns The value of the Last Conversion
 */
static inline uint16_t hal_adc_readConversion(ADC_TypeDef *adc){
    CLEAR_BIT(ADC1->SR, ADC_SR_EOC);
    return adc->DR & ADC_DR_DATA;
}

