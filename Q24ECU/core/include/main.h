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
#include "taskHandlers.h"
#include "nvicConfig.h"
#include "interrupts.h"

// Comment this out to remove task names from the debug print
#define DEBUG_PRINTF_TASK_NAME


extern void SystemInit(void);

extern int main(void);
