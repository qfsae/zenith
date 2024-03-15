/**
 * @file controller.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief System Task Controller
 * @version 0.1
 * @date 2024-03-14
 * 
 * This file is responsible for housing management functions for the tasks
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "sys_tasks.h"

TaskHandle_t xTaskHandles[eTask_TaskCount];

StaticTask_t xTaskBuffers[eTask_TaskCount];

StackType_t xTaskStacks[eTask_TaskCount][configMINIMAL_STACK_SIZE];

void task_init(enum Tasks eTask, TaskFunction_t pvTask, const char* pcName){
    // Create the task and assign the taskHandle
    xTaskHandles[eTask] = xTaskCreateStatic(
        pvTask,
        pcName,
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        xTaskStacks[eTask],
        &xTaskBuffers[eTask]
    );
    // Suspend the task after creation
    vTaskSuspend(xTaskHandles[eTask]);
}


void tasker_init(void){
    // Create Software BSPD Task
    task_init(eTask_SBSPD, vTask_SBSPD, "SBSPD");

    // Create Software BSPD Task
    task_init(eTask_ErrorScreen, vTask_ErrorScreen, "ErrDisp");

    // Create Test 1 Task
    task_init(eTask_TEST1, vTask_Test1, "Test1");
    
    // Create Test 1 Task
    task_init(eTask_CANSend, vTask_CAN_send, "CANTX");

}

