/**
 * @file taskHandlers.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Initialize All Tasks
 * @version 0.1
 * @date 2023-11-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "taskHandlers.h"
#include <stdio.h>

// Init global handles for tasks

TaskHandle_t xTaskHandles[eTask_TaskCount];

StaticTask_t xTaskBuffers[eTask_TaskCount];

StackType_t xTaskStacks[eTask_TaskCount][configMINIMAL_STACK_SIZE];

void os_task_init(void){

    // Create Task to empty CAN RX Buffer (Part of interface_can.c)
    xTaskHandles[eTask_CAN_rxBufferHandler] = xTaskCreateStatic(
        vTask_CAN_RXBufferHandler,
        "CAN IRQ RX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+2,
        xTaskStacks[eTask_CAN_rxBufferHandler],
        &xTaskBuffers[eTask_CAN_rxBufferHandler]
    );

    // Create Sample Blink Task
    xTaskHandles[eTask_Test1] = xTaskCreateStatic(
        vTask_Test1,
        "TEST1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_Test1],
        &xTaskBuffers[eTask_Test1]
    );
    // Create Sample Print Task
    xTaskHandles[eTask_BlinkLED] = xTaskCreateStatic(
        vTask_BlinkLED,
        "BLINK",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_BlinkLED],
        &xTaskBuffers[eTask_BlinkLED]
    );

    // Create Sample CAN TX task
    xTaskHandles[eTask_CAN_send] = xTaskCreateStatic(
        vTask_CAN_send,
        "CANtx",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_CAN_send],
        &xTaskBuffers[eTask_CAN_send]
    );

    // Create Sample CAN RX Task
    xTaskHandles[eTask_CAN_receive] = xTaskCreateStatic(
        vTask_CAN_receive,
        "CANrx",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_CAN_receive],
        &xTaskBuffers[eTask_CAN_receive]
    );

    xTaskHandles[eTask_USART2_Handler] = xTaskCreateStatic(
        vTask_USART2_Handler,
        "USART2 IRQ RX",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_USART2_Handler],
        &xTaskBuffers[eTask_USART2_Handler]
    );

}

