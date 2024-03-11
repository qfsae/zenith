/**
 * @file tasks.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Source file for the central collection of all system tasks
 * @version 0.1
 * @date 2024-03-10
 * 
 * All tasks should be statically declared within the tasks files
 * All tasks should be initialized to a suspended state and resumed by the core system
 * Test tasks can be deleted with `vTaskDelete()` to ensure they are not running
 * 
 * DO NOT use `vTaskSuspendAll()` as it could interfere with the core task
 *
 * @copyright Copyright (c) 2024
 * 
 */

#include "tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"

// Init global handles for tasks

TaskHandle_t xTaskHandles[eTask_TaskCount];

StaticTask_t xTaskBuffers[eTask_TaskCount];

StackType_t xTaskStacks[eTask_TaskCount][configMINIMAL_STACK_SIZE];


void task_init(void){
    // Create Software BSPD Task
    xTaskHandles[eTask_SBSPD] = xTaskCreateStatic(
        vTask_SBSPD,
        "SBSPD",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_SBSPD],
        &xTaskBuffers[eTask_SBSPD]
    );
    // Suspend the task after creation
    vTaskSuspend(xTaskHandles[eTask_SBSPD]);

    // Create Software BSPD Task
    xTaskHandles[eTask_ErrorScreen] = xTaskCreateStatic(
        vTask_ErrorScreen,
        "ErrDisp",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask_ErrorScreen],
        &xTaskBuffers[eTask_ErrorScreen]
    );
    // Suspend the task after creation
    vTaskSuspend(xTaskHandles[eTask_ErrorScreen]);
}

void task_printDebug(UART_Handle_t *Serial){
    char buf[100];
    snprintf(buf, 100, "Task:\t   Identifier\t    Priority\t\tState\n");
    uart_write(Serial, buf, strlen(buf), 10);
    for (int i = 0; i < eTask_TaskCount; i++)
    {
        snprintf(buf, 100, "%d: %16s\t\t%lu\t\t", i, pcTaskGetName(xTaskHandles[i]), uxTaskPriorityGet(xTaskHandles[i]));
    uart_write(Serial, buf, strlen(buf), 10);
        switch(eTaskGetState(xTaskHandles[i])) {
            case eRunning:
                uart_write(Serial, "RUNNING\n", 9, 10);
                break;
            case eReady:
                uart_write(Serial, "READY\n", 6, 10);
                break;
            case eBlocked:
                uart_write(Serial, "BLOCKED\n", 8, 10);
                break;
            case eSuspended:    
                uart_write(Serial, "SUSPENDED\n", 10, 10);
                break;
            case eDeleted:   
                uart_write(Serial, "DELETED\n", 8, 10);
                break;
            case eInvalid:   
                uart_write(Serial, "INVALID\n", 8, 10);
                break;
            default:
                uart_write(Serial, "UNKNOWN\n", 8, 10);
                break;
        }
    }
}
