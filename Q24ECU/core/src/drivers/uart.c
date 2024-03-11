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
#include "nvicConfig.h"


// UART OS Handlers
UART_Handle_t Serial2 = {
    .pUART  = USART2,
    .writeSemaphore = NULL,
    .writeHandler = NULL,
    .readSemaphore = NULL,
    .readHandler  = NULL,
    .pBuffer = NULL
};


UART_Handle_t Serial4 = {
    .pUART  = UART4,
    .writeSemaphore = NULL,
    .writeHandler = NULL,
    .readSemaphore = NULL,
    .readHandler  = NULL,
    .pBuffer = NULL
};


void uart_init(unsigned long baud){
    /* BEGIN Init USART 2 */
    hal_uart_init(Serial2.pUART, baud, PIN_USART2_TX, PIN_USART2_RX);
    Serial2.writeHandler = xSemaphoreCreateBinaryStatic(&Serial2.writeSemaphore);
    if(Serial2.writeHandler != NULL)
        xSemaphoreGive(Serial2.writeHandler);
    Serial2.readHandler = xSemaphoreCreateBinaryStatic(&Serial2.readSemaphore);
    if(Serial4.readHandler != NULL)
        xSemaphoreGive(Serial2.readHandler);
    /* END Init USART 2 */
    
    /* BEGIN Init USART 4 */
    hal_uart_init(Serial4.pUART, baud, PIN_UART4_TX, PIN_UART4_RX);
    Serial4.writeHandler = xSemaphoreCreateBinaryStatic(&Serial4.writeSemaphore);
    if(Serial4.writeHandler != NULL)
        xSemaphoreGive(Serial4.writeHandler);
    Serial4.readHandler = xSemaphoreCreateBinaryStatic(&Serial4.readSemaphore);
    if(Serial4.readHandler != NULL)
        xSemaphoreGive(Serial4.readHandler);
    /* END Init USART 4 */
    
}


enum SYS_ERROR uart_open_buffer(UART_Handle_t *pHandle, UART_StreamBuffer_t *stream, TickType_t timeout) {
    return UART_UNINIT_ERR;
}

enum SYS_ERROR uart_close_buffer(UART_Handle_t *pHandle){
    return UART_UNINIT_ERR;
}

enum SYS_ERROR uart_write_byte(UART_Handle_t *pHandle, char byte, TickType_t timeout){
    if(pHandle == NULL) return UART_UNINIT_ERR;
    if(pHandle->writeHandler == NULL) return UART_UNINIT_ERR;
    if(xSemaphoreTake(pHandle->writeHandler, timeout) == pdTRUE){
        hal_uart_write_byte(pHandle->pUART, byte);
        xSemaphoreGive(pHandle->writeHandler);
        return SYS_OK;
    }
    return UART_ACC_ERR;
}

enum SYS_ERROR uart_write(UART_Handle_t *pHandle, char* buf, size_t len, TickType_t timeout){
    if(pHandle == NULL) return UART_UNINIT_ERR;
    if(pHandle->writeHandler == NULL) return UART_UNINIT_ERR;
    if(xSemaphoreTake(pHandle->writeHandler, timeout) == pdTRUE){
        hal_uart_write_buf(pHandle->pUART, buf, len);
        xSemaphoreGive(pHandle->writeHandler);
        return SYS_OK;
    }
    return UART_ACC_ERR;
}

