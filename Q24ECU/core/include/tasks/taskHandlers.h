/**
 * @file taskHandlers.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
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
    eTask_SysError,
    eTask_ADCMonitor,
    // Keep this last
    eTask_TaskCount // Number of tasks running on the system (`taskHandlers.h`)
};

extern TaskHandle_t xTaskHandles[eTask_TaskCount];

/**
 * @brief Called by the runner to initialize all tasks
 * 
 */
extern void runner_task_init(void);

/**
 * @brief Can be called by the runner to print out task information
 * 
 */
extern void runner_displayInfo(void);

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

// Located in interface_sysError.c
extern void vTask_SysError(void *param);

// Located in interface_adc.c
extern void vTask_ADCMonitor(void *param);
