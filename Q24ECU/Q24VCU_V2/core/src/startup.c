/**
 * @file startup.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief System Startup
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"

/**
 * @brief Initializes the CPU
 * 
 */
void SystemInit(void){
    // Initialize Clock
    clock_init();
    // Enable system configuration register
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // enable FPU
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
    // configure system tick (ARM)
    SysTick_Config(SYS_FREQUENCY/1000); // DO NOT USE "SystemClock"
}