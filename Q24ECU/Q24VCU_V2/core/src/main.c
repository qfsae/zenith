/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * 
 */

#include "main.h"

static uint16_t led1 = PIN('B', 0);
static uint16_t led2 = PIN('B', 1);

static volatile uint32_t s_ticks = 0xBEEF;

void SysTick_Handler(void){
    s_ticks++;
}

void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(led1);
}

int main(void){
    // set system ticks to zero
    s_ticks = 0x0;

    // set up gpio
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);

    // set up main loop delay
    volatile uint32_t timer = 0, period = 1000;

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    for(;;) {
        if(timer_expired(&timer, period, s_ticks)){
            gpio_toggle_pin(led2);
        }
        //gpio_toggle_pin(led1);
    }
    return 0;
}