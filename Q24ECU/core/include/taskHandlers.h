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

extern TaskHandle_t tskh_Test1;
extern TaskHandle_t tskh_BlinkLED;
extern TaskHandle_t tskh_USART2_Handler;