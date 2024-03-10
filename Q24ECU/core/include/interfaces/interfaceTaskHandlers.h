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

enum InterfaceTasks{
    eTask_USART2_Handler,
    eTask_CAN_rxBufferHandler,
    eTask_SysError,
    eTask_ADCMonitor,
    // Keep this last
    eTask_TaskCount // Number of tasks running on the system (`taskHandlers.h`)
};

extern TaskHandle_t xInterfaceTaskHandles[eTask_TaskCount];

/**
 * @brief Called by the runner to initialize all tasks used by interfaces
 * 
 */
extern void interface_task_init(void);

// Located in interface_uart.c
extern void vTask_USART2_Handler(void *param);

// Located in interface_can.c
extern void vTask_CAN_RXBufferHandler(void *param);

// Located in interface_sysError.c
extern void vTask_SysError(void *param);

// Located in interface_adc.c
extern void vTask_ADCMonitor(void *param);

