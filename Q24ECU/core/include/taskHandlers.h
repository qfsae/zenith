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

#pragma once

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// Test Task 1
extern TaskHandle_t tskh_Test1;
// Blink LED example Task
extern TaskHandle_t tskh_BlinkLED;
// UART2 (debug) receive task handler
extern TaskHandle_t tskh_USART2_Handler;
// CAN RX Stream Handler
extern TaskHandle_t tskh_CANRX_Handler;
// CAN Runner transmit task
extern TaskHandle_t tskh_CAN_send;
// CAN Runner recieve task
extern TaskHandle_t tskh_CAN_recieve;

extern void os_task_init(void);

// Located in testTasks.c
extern void tsk_Test1(void *param);

// Located in testTasks.c
extern void tsk_BlinkLED(void *param);

// Located in interface_uart.c
extern void tsk_USART2_Handler(void *param);

// Located in interface_can.c
extern void tsk_CAN_RXBufferHandler(void *param);

// Located in canRunner.c
extern void tsk_CAN_send(void *param);

// Located in canRunner.c
extern void tsk_CAN_recieve(void *param);

