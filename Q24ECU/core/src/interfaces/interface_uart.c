/**
 * @file interface_uart.c
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-11-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "interfaces/interface_uart.h"
#include "FreeRTOS.h"
#include "task.h"


uart_t debug;

void uart_setup(){
    // Enable the debug usart and setup its IQR handler
    uart_send_init(&debug, UART_DEBUG, 250000);
    // hal_uart_enable_rxne(debug.interface, true);
    // NVIC_SetPriority(USART2_IRQn, 0x03);
    // NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t receivedData;

        if(hal_uart_read_ready(debug.interface)){
            uint8_t receivedData;

            // Read data from the USART (replace with your actual read operation)
            receivedData = hal_uart_read_byte(debug.interface);

            // Send the received data to the task
            xQueueSendFromISR(debug.rxQue, (void*) &receivedData, NULL);
        }
    // /* Now the buffer is empty we can switch context if necessary. */
    // if( xHigherPriorityTaskWoken )
    // {
    //     /* Actual macro used here is port specific. */
    //     portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    // }
    
}