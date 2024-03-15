
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
#include "task.h"
#include "interfaces/cm200dx.h"
#include "sys_tasks.h"
#include "drivers/can.h"
#include <stdio.h>
#include <string.h>

void vTask_CAN_send(void *param){
    (void)param;
    vTaskDelay(1);
    uart_write_str(&Serial2, "Running CAN TX Task from canRunner.c\n", 1000U);
    can_msg_t tx_msg;
    tx_msg.id = 33;
    tx_msg.len = 1;
    tx_msg.data[0] = 0;

    for(;;){
        // char buf[40];
        // snprintf(buf, 40, "Sending: %d\n", tx_msg.data[0]);
        // uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 1000);
        can_write(CAN1, &tx_msg, portMAX_DELAY);
        cm200dx_setPower(RIGHT, 200);
        tx_msg.data[0]++;
        if(tx_msg.data[0] > 200) tx_msg.data[0] = 0;
        vTaskDelay(200);
    }
}

// void vTask_CAN_receive(void *param){
//     (void)param;
//     vTaskDelay(1);
//     printf("Initializing RX\n");
//     uart_send_buf_blocking(&port_uart4, "Running CAN RX Task from canRunner.c\n", 38U, 1000U);    
//     can_msg_t rx_msg;
//     for(;;){
//         rx_msg = can_fetch(CAN1, 200);
//         char buf[40];
//         snprintf(buf, 40, "%ld: %d\n", rx_msg.id, rx_msg.data[0]);
//         // uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 1000);
//         vTaskDelay(100);
//     }
// }

