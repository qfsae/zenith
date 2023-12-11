/**
 * @file TIM6.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Timer 6 Interrupt Request Handler
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 * The Timer 6 Interrupt is designed to alert the operator to a full system lockup.
 * If the debug LED on the test board stops blinking at a 1 hz interval, then the CPU
 * is deadlocked. The debug LED can also be used as a visual reference to see that the
 * board is online and running at the correct speed.
 * 
 */

#include "main.h"
#include "hal/hal_gpio.h"

void TIM6_DAC_IRQHandler(void){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(debug_led1);
}

