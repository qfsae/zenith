
/**
 * @file canRunner.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief CAN Runner - Sample Interaction with CAN Bus
 * @version 0.1
 * @date 2024-1-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "main.h"
#include "taskHandlers.h"
#include "task.h"
#include "interfaces/interface_can.h"
#include <stdio.h>

void tsk_CAN_send(void *param){
    (void)param;
    printf("Running CAN Send Task from canRunner.c\n");
    can_msg_t tx_msg;
    tx_msg.id = 33;
    tx_msg.len = 1;
    tx_msg.data[0] = 0;

    for(;;){
        can_send_msg(CAN1, &tx_msg, portMAX_DELAY);
        tx_msg.data[0]++;
        if(tx_msg.data[0] > 200) tx_msg.data[0] = 0;
        vTaskDelay(100);
    }
}

void tsk_CAN_recieve(void *param){
    (void)param;
    printf("Running CAN Recieve Task from canRunner.c\n");
    can_msg_t rx_msg;
    for(;;){
        rx_msg = can_fetch(CAN1, 200);
        printf(">RPM: %d\n", rx_msg.data[0]);
        vTaskDelay(100);
    }
}

