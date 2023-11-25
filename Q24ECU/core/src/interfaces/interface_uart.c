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
#include "taskHandlers.h"
#include "nvicConfig.h"

// UART OS Handlers
uart_t port_uart2;


/**
 * @brief OS UART Setup handler
 * 
 * This function should be called to initalized all onboard UART interfaces.
 * This function should not be called more than once.
 * All UART initialization should happen within this function
 * 
 * Called in main
 * 
 */
void os_uart_setup(){
    // Enable the UART 2 port and setup its IQR handler
    uart_send_init(&port_uart2, USART2, 250000);
    xSemaphoreGive(port_uart2.semaphore);
    hal_uart_enable_rxne(port_uart2.port, true);
    NVIC_SetPriority(USART2_IRQn, (NVIC_Priority_MIN-10));
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(){
    // Initialize variable to trigger context switch to false (no context switch)
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Receive the data loaded into the UART DR (data register)
    uint8_t receivedData = 0;
    // use the uart 2 CMSIS define (reduce risk of hanging interrupt)
    receivedData = hal_uart_read_byte(USART2);

    // Add the received data into the rx buffer stream
    xStreamBufferSendFromISR(port_uart2.rxbuffer, &receivedData, sizeof(receivedData), &xHigherPriorityTaskWoken);

    // Check and trigger a context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}