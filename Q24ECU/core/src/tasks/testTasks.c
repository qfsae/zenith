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
    // printf("setup");
    for(;;){
        //incoming = can_fetch(CAN1, 200);
        //printf(">RPM:%d\n", incoming.data[0]);
        //incoming.len = 1;
        //incoming.data[0] = 232;
        //incoming.id = 99;
        //can_send_msg(CAN1, &incoming, portMAX_DELAY);
        //can_send_msg(CAN1, &incoming);
        //hal_can_send(CAN1, &incoming, 2);
        //vTaskDelay(10);
        //incoming.id = 100;
        //hal_can_send(CAN1, &incoming, 1);
        //while(!hal_can_send_ready(CAN1, 2)) asm("nop");
        //while(!hal_can_send_ready(CAN1, 1)) asm("nop");
        vTaskDelay(200);
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
        can_msg_t msg;
        msg.id = 98;
        msg.len = 1;
        msg.data[0] = 22;
        //hal_can_send(CAN1, &msg, 1);
        can_send_msg(CAN1, &msg, portMAX_DELAY);
        //while(hal_can_send_ready(CAN1, 1));
        vTaskDelayUntil(&lastWakeTime, 100);
    }
}

