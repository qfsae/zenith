
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
#include "interfaces/interface_uart.h"
#include <stdio.h>
#include <string.h>

void vTask_CAN_send(void *param){
    (void)param;
    
    // printf("Running CAN Send Task from canRunner.c\n");
    uart_send_buf_blocking(&port_uart4, "Running CAN TX Task from canRunner.c\n", 38U, 1000);
    can_msg_t tx_msg;
    tx_msg.id = 33;
    tx_msg.len = 1;
    tx_msg.data[0] = 0;

    for(;;){
        // char buf[40];
        // snprintf(buf, 40, "Sending: %d\n", tx_msg.data[0]);
        // uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 1000);
        // can_send_msg(CAN1, &tx_msg, portMAX_DELAY);
        // tx_msg.data[0]++;
        // if(tx_msg.data[0] > 200) tx_msg.data[0] = 0;
    for (int i = 0; i < eTask_TaskCount; i++)
    {
        printf("Task %d: %s\t", i, pcTaskGetName(xTaskHandles[i]));
        switch(eTaskGetState(xTaskHandles[i])) {
            case eRunning:
                printf("RUNNING\n");
                break;
            case eReady:
                printf("READY\n");
                break;
            case eBlocked:
                printf("BLOCKED\n");
                break;
            case eSuspended:    
                printf("SUSPENDED\n");
                break;
            case eDeleted:   
                printf("DELETED\n");
                break;
            case eInvalid:   
                printf("INVALID\n");
                break;
            default:
                printf("UNKNOWN\n");
                break;
        }
    }
        vTaskDelay(2000);
        printf("\033[2J");
    }
}

void vTask_CAN_receive(void *param){
    (void)param;
    printf("Running CAN Recieve Task from canRunner.c\n");
    //hal_uart_write_buf(UART4, "Running CAN RX Task from canRunner.c\n", 38U);
    hal_uart_init(UART4, 115200, PIN_UART4_RX, PIN_UART4_TX);
    hal_uart_write_buf(UART4, "Running CAN RX Task from canRunner.c\n", 38U);
    
    can_msg_t rx_msg;
    for(;;){
        // rx_msg = can_fetch(CAN1, 200);
        hal_uart_write_byte(UART4, 'H');
        hal_uart_write_byte(UART4, 'E');
        hal_uart_write_byte(UART4, 'L');
        hal_uart_write_byte(UART4, 'L');
        hal_uart_write_byte(UART4, 'O');
        hal_uart_write_byte(UART4, '\n');
        // printf(">RPM: %d\n", rx_msg.data[0]);
        // char buf[40];
        // snprintf(buf, 40, "RPM: %d\n", rx_msg.data[0]);
        // uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 1000);
        vTaskDelay(100);
    }
}

