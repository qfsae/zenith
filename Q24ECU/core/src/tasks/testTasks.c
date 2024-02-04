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
#include "interfaces/interface_can.h"
#include <stdio.h>

long unsigned int counter = 0;

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    printf("Running Task 1\n");
    can_msg_t incoming;
    // printf("setup");
    for(;;){
        incoming = can_fetch(CAN1, 200);
        printf(">RPM:%d\n", incoming.data[0]);
        incoming.len = 1;
        incoming.id = 99;
        can_send_msg(CAN1, &incoming, portMAX_DELAY);
        vTaskDelay(200);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for(;;){
        // This task is currently not used so perma suspend it
        lastWakeTime = xTaskGetTickCount();
        counter++;
        can_msg_t msg;
        msg.id = 98;
        msg.len = 1;
        msg.data[0] = 22;
        can_send_msg(CAN1, &msg, portMAX_DELAY);
        vTaskDelayUntil(&lastWakeTime, 100);
    }
}
