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


uart_t debug;

void uart_setup(){
    // Enable the debug usart and setup its IQR handler
    uart_send_init(&debug, UART_DEBUG, 250000);
    hal_uart_enable_rxne(debug.interface, true);
    NVIC_SetPriority(USART2_IRQn, 0x03);
    NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t receivedData = 0;
    // Clear the interrupt bit (enables exiting of fn)
    // xSemaphoreTakeFromISR(debug.handle, &xHigherPriorityTaskWoken);
    receivedData = hal_uart_read_byte(UART_DEBUG);
    // xSemaphoreGiveFromISR(debug.handle, &xHigherPriorityTaskWoken);

    xStreamBufferSendFromISR(debug.stream, &receivedData, sizeof(receivedData), NULL);

    // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    if(receivedData == '1'){
        gpio_toggle_pin(debug_led2);
    }
}