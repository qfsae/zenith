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

TaskHandle_t tskh_Test1 = NULL;
TaskHandle_t tskh_BlinkLED = NULL;
TaskHandle_t tskh_USART2_Handler = NULL;
TaskHandle_t tskh_CANRX_Handler = NULL;
TaskHandle_t tskh_CAN_send = NULL;
TaskHandle_t tskh_CAN_recieve = NULL;

void os_task_init(void){
    // Create Sample Blink Task
    (void)xTaskCreate(
        tsk_BlinkLED,
        "blink",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_BlinkLED
    );
    // Create Sample Print Task
    (void)xTaskCreate(
        tsk_Test1,
        "tst1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+2,
        &tskh_Test1
    );

    // Create Sample CAN TX task
    (void)xTaskCreate(
        tsk_CAN_send,
        "cRtx",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_CAN_send
    );

    // Create Sample CAN RX Task
    (void)xTaskCreate(
        tsk_CAN_recieve,
        "cRrx",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_CAN_recieve
    );
}

