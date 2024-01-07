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
    for(;;){
        incoming = can_fetch(CAN1, 0x119);
        printf("RPM: %d\n", incoming.data[0]);
        vTaskDelay(10);
    }
    // // printf("Starting CAN Init..\n");
    // int can_status = hal_can_init(CAN1, CAN_1000KBPS, true, PIN('A', 11), PIN('A', 12));
    // hal_CAN_msg_t rx_msg, tx_msg;
    // tx_msg.len = 0;
    // tx_msg.data[0] = 0;
    // tx_msg.format = STANDARD_FORMAT;
    // tx_msg.type = DATA_FRAME;
    // tx_msg.id = 0;
    // // printf("CAN init Finished: %d\n", can_status);
    // for (;;)
    // {
    //     tx_msg.id++;
    //     if(tx_msg.id > 999) tx_msg.id = 0;
    //     // printf("Sending MSG");
    //     hal_can_send(CAN1, &tx_msg);
    //     while(hal_can_send_ready(CAN1) == 0) vTaskDelay(10);
    //     // printf("\n");
    //     vTaskDelay(10);
    //     // Print out the systemtick timer once a second
    //     // printf("%ld\n", (long)xTaskGetTickCount());
    //     // vTaskDelay(1000);
    // }
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

