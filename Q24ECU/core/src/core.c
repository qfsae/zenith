/**
 * @file core.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Main System Runtime Header
 * @version 0.1
 * @date 2024-03-04
 * 
 * This file serves as the header file for the main system runtime
 * Runner is responsible for system startup, excluding the bootloader, and hardware interfaces
 * that could prevent the system from starting up properly.
 * 
 * Runner's main function is to initialize and maintain the acceleration throttle control (ATC)
 * tasks including the ADT task itself and the Software Differential Control (SDC) task.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "stdio.h"
#include "core.h"
#include "tasks/taskHandlers.h"

#define taskResume(tsk) vTaskResume(xTaskHandles[tsk])
#define taskSuspend(tsk) vTaskSuspend(xTaskHandles[tsk])

enum SYS_STATE core_state = SYS_STATE_INIT;

void vTask_Core(void *param){
    (void) param;
    
    vTaskDelay(1);
    printf("Runner Initalized...\nInitializing Tasks...\n");
    taskENTER_CRITICAL();

    // Initialize the system Tasks
    runner_task_init();

    // Ensure that all tasks are disabled on startup
    for (int i = 0; i < eTask_TaskCount; i++)
        taskSuspend(i);

    // Manually resume all critical tasks
    taskResume(eTask_ADCMonitor);
    taskResume(eTask_CAN_rxBufferHandler);
    taskResume(eTask_SysError);
    taskResume(eTask_USART2_Handler);
    // Display the info for all tasks on the system
    runner_displayInfo();
    
    
}


