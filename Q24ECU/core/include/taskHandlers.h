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

enum Tasks{
    eTask_Test1,
    eTask_BlinkLED,
    eTask_USART2_Handler,
    eTask_CAN_send,
    eTask_CAN_receive,
    eTask_CAN_rxBufferHandler,
    eTask_TaskCount // Keep this last
};

extern TaskHandle_t xTaskHandles[eTask_TaskCount];

extern void os_task_init(void);

// Located in testTasks.c
extern void vTask_Test1(void *param);

// Located in testTasks.c
extern void vTask_BlinkLED(void *param);

// Located in interface_uart.c
extern void vTask_USART2_Handler(void *param);

// Located in interface_can.c
extern void vTask_CAN_RXBufferHandler(void *param);

// Located in canRunner.c
extern void vTask_CAN_send(void *param);

// Located in canRunner.c
extern void vTask_CAN_receive(void *param);

