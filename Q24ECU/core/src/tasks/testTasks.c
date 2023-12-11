/**
 * @file testTasks.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Test Task file - Collection of test tasks
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "taskHandlers.h"
#include "main.h"
#include <stdio.h>

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        // Print out the systemtick timer once a second
        printf("%ld\n", (long)xTaskGetTickCount());
        vTaskDelay(1000);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    for(;;){
        // This task is currently not used so perma suspend it
        vTaskSuspend(NULL);
    }
}

