/**
 * @file taskHandlers.h
 * @author Jacob Chisholm
 * @brief Global definitions of all task handlers defined in main.c
 * @version 0.1
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// Test Task 1
extern TaskHandle_t tskh_Test1;
// Blink LED example Task
extern TaskHandle_t tskh_BlinkLED;
// UART2 (debug) receive task handler
extern TaskHandle_t tskh_USART2_Handler;