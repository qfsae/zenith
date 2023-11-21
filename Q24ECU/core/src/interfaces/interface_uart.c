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


uart_t debug;

void uart_setup(){
    // Enable the debug usart and setup its IQR handler
    uart_send_init(&debug, UART_DEBUG, 250000);
    hal_uart_enable_rxne(debug.interface, true);
    NVIC_SetPriority(USART2_IRQn, (NVIC_Priority_MIN-10));
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t receivedData = 0;
    receivedData = hal_uart_read_byte(UART_DEBUG);

    xStreamBufferSendFromISR(debug.stream, &receivedData, sizeof(receivedData), &xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}