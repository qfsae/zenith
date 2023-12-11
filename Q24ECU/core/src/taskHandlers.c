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

// Init global handles for tasks

TaskHandle_t tskh_Test1 = NULL;
TaskHandle_t tskh_BlinkLED = NULL;
TaskHandle_t tskh_USART2_Handler = NULL;

void os_task_init(void){
    // Create Sample Blink Task
    xTaskCreate(
        tsk_BlinkLED,
        "blink",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_BlinkLED
    );

    // Create Sample Print Task
    xTaskCreate(
        tsk_Test1,
        "tst1",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_Test1
    );
}

