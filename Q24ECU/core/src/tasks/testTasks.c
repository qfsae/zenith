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
#include "FreeRTOS.h"
#include "task.h"
#include "hal/hal_can.h"

long unsigned int counter = 0;

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    CAN_msg_t rx_msg;
    for(;;){
        if(CANMsgAvail(CAN1)){
        hal_can_receive(CAN1, &rx_msg);
        printf("ID: %d\n", rx_msg.data[0]);
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

