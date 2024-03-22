/**
 * @file hal_tim_basic.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief HAL Library for STM32 Basic Timer
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stm32f4xx.h>


/**
 * @brief Init function for basic timers (TIM6/TIM7)
 * 
 * @param timer TIM6 or TIM6 (STM32F446)
 * @param prescaler Clock Prescaler
 * @param reload_register Overflow Evaluator
 * Note: Values loaded into prescaler and reload are (value-1) -> see datasheet
 */
static inline void TIM_basic_Init(TIM_TypeDef *timer, uint16_t prescaler, uint16_t reload_register){
    // Enable Clock
    if(timer == TIM6) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    if(timer == TIM7) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    // Reset and Set Master Mode to Enable (CNT_EN is used as a trigger output)
    timer->CR2 &= ~(7UL << 4U);
    timer->CR2 |= BIT(4); // set timer master mode to enable
    timer->DIER &= ~(BIT(8U) | BIT(0U)); // reset interrupt enable reg
    timer->DIER |= BIT(0); // enable timer interrupt
    // TIM6->EGR |= BIT(0);
    timer->ARR = (uint32_t)(reload_register-1UL);//(65536);
    timer->PSC = (uint32_t)(prescaler-1UL);//(APB1_FREQUENCY);
    timer->CR1 &= ~(TIM_CR1_ARPE | TIM_CR1_OPM | TIM_CR1_URS | TIM_CR1_CEN); // reset timer control register 1
    timer->CR1 |= (TIM_CR1_CEN); // set interrupt source to only overflow | enable timer
}

