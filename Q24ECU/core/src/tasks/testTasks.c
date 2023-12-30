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
#include "task.h"
#include "hal/hal_can.h"
#include <stdio.h>

long unsigned int counter = 0;

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    printf("Starting CAN Init..\n");
    int can_status = hal_can_init(CAN1, CAN_1000KBPS, true, PIN('A', 11), PIN('A', 12));
    hal_CAN_msg_t msg;
    printf("CAN init Finished: %d\n", can_status);
    for (;;)
    {
        if(hal_can_read_ready(CAN1)){
            hal_can_receive(CAN1, &msg);
            printf("ID: %d LEN: %d Data: ", msg.id, msg.len);
            for (int i = 0; i < msg.len; i++)
            {
                printf(" %d ", msg.data[i]);
            }
            printf("\n");
        }
        vTaskDelay(10);
        // Print out the systemtick timer once a second
        // printf("%ld\n", (long)xTaskGetTickCount());
        // vTaskDelay(1000);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for(;;){
        // This task is currently not used so perma suspend it
        lastWakeTime = xTaskGetTickCount();
        counter++;
        vTaskDelayUntil(&lastWakeTime, 10);
    }
}

