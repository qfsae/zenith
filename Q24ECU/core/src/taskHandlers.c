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

void os_task_init(void){
    // Create Sample Blink Task
    xTaskCreate(
        tsk_BlinkLED,
        "blink",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_BlinkLED
    );
    printf("starting test task\n");
    // Create Sample Print Task
    int i = xTaskCreate(
        tsk_Test1,
        "tst1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+2,
        &tskh_Test1
    );
    printf("Started Task: %d", i);
}

