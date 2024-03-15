/**
 * @file main.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stm32f4xx.h>
#include "sys_tasks.h"
#include "sys_state.h"
#include "nvicConfig.h"

// Comment this out to remove task names from the debug print
#define DEBUG_PRINTF_TASK_NAME


/* Interrupts globally defined for linker */

// Timer 6 IRQn
extern void TIM6_DAC_IRQHandler(void);

// Located in shutdown.c
extern void system_critical_shutdown(void);

extern void SystemInit(void);

extern int main(void);
