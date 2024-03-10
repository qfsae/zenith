/**
 * @file uart.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Source file for UART Driver
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "drivers/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks/taskHandlers.h"
#include "nvicConfig.h"


// UART OS Handlers
struct uart_port port_uart2 = {
    .port  = USART2,
    .writeSemaphore = NULL,
    .writeHandler = NULL,
    .readSemaphore = NULL,
    .readHandler  = NULL,
    .pBuffer = NULL
};


struct uart_port port_uart4 = {
    .port  = UART4,
    .writeSemaphore = NULL,
    .writeHandler = NULL,
    .readSemaphore = NULL,
    .readHandler  = NULL,
    .pBuffer = NULL
};



void uart_init(unsigned long baud){
    /* BEGIN Init USART 2 */
    hal_uart_init(port_uart2.port, baud, PIN_USART2_TX, PIN_USART2_RX);
    port_uart2.writeHandler = xSemaphoreCreateBinaryStatic(&port_uart2.writeSemaphore);
    if(port_uart2.writeHandler != NULL)
        xSemaphoreGive(port_uart2.writeHandler);
    port_uart2.readHandler = xSemaphoreCreateBinaryStatic(&port_uart2.readSemaphore);
    if(port_uart2.readHandler != NULL)
        xSemaphoreGive(port_uart2.readHandler);
    /* END Init USART 2 */
    
    /* BEGIN Init USART 4 */
    hal_uart_init(port_uart4.port, baud, PIN_UART4_TX, PIN_UART4_RX);
    port_uart4.writeHandler = xSemaphoreCreateBinaryStatic(&port_uart4.writeSemaphore);
    if(port_uart4.writeHandler != NULL)
        xSemaphoreGive(port_uart4.writeHandler);
    port_uart4.readHandler = xSemaphoreCreateBinaryStatic(&port_uart4.readSemaphore);
    if(port_uart4.readHandler != NULL)
        xSemaphoreGive(port_uart4.readHandler);
    /* END Init USART 4 */
    
}


enum SYS_ERROR uart_open_buffer(struct uart_port *port, struct uart_streamBuffer *stream, TickType_t timeout) {
    return UART_UNINIT_ERR;
}

enum SYS_ERROR uart_close_buffer(struct uart_port *port){
    return UART_UNINIT_ERR;
}

enum SYS_ERROR uart_write_byte(struct uart_port *port, char byte, TickType_t timeout){
    if(port == NULL) return UART_UNINIT_ERR;
    if(port->writeHandler == NULL) return UART_UNINIT_ERR;
    if(xSemaphoreTake(port->writeHandler, timeout) == pdTRUE){
        hal_uart_write_byte(port->port, byte);
        xSemaphoreGive(port->writeHandler);
        return SYS_OK;
    }
    return UART_ACC_ERR;
}

enum SYS_ERROR uart_write(struct uart_port *port, char* buf, size_t len, TickType_t timeout){
    if(port == NULL) return UART_UNINIT_ERR;
    if(port->writeHandler == NULL) return UART_UNINIT_ERR;
    if(xSemaphoreTake(port->writeHandler, timeout) == pdTRUE){
        hal_uart_write_buf(port->port, buf, len);
        xSemaphoreGive(port->writeHandler);
        return SYS_OK;
    }
    return UART_ACC_ERR;
}



