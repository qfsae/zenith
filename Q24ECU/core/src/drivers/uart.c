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
#include "semphr.h"
#include <string.h>


// UART OS Handlers
UART_Handle_t Serial2 = {
    .pUART  = USART2,
    .writeHandler = NULL,
    .readHandler  = NULL,
    .pBuffer = NULL
};


UART_Handle_t Serial4 = {
    .pUART  = UART4,
    .writeHandler = NULL,
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
    if(pHandle == NULL) return SYS_INVALID_ARG;
    if(stream == NULL) return SYS_INVALID_ARG;
    if(stream->handle == NULL) return SYS_INVALID_ARG;
    if(xSemaphoreTake(pHandle->readHandler, timeout)){
        hal_uart_enable_rxne(pHandle->pUART, true);
        pHandle->pBuffer = &stream->handle;
        return SYS_OK;
    }
    return UART_UNINIT_ERR;
}

enum SYS_ERROR uart_close_buffer(UART_Handle_t *pHandle){
    if(pHandle == NULL) return SYS_INVALID_ARG;
    if(xSemaphoreGetMutexHolder(pHandle->readHandler) == xTaskGetCurrentTaskHandle()){
        hal_uart_enable_rxne(pHandle->pUART, false);
        pHandle->pBuffer = NULL;
    }
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


enum SYS_ERROR uart_write_str(UART_Handle_t *pHandle, char* str,  TickType_t timeout){
    if(pHandle == NULL) return UART_UNINIT_ERR;
    if(pHandle->writeHandler == NULL) return UART_UNINIT_ERR;
    if(xSemaphoreTake(pHandle->writeHandler, timeout) == pdTRUE){
        hal_uart_write_buf(pHandle->pUART, str, strlen(str));
        xSemaphoreGive(pHandle->writeHandler);
        return SYS_OK;
    }
    return UART_ACC_ERR;
}

/* BEGIN UART Interrupt Request Handlers */

void USART2_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t rxData = hal_uart_read_byte(Serial2.pUART);
    // Null check stream buffer (disable IRQ and return on failure)
    if(Serial2.pBuffer == NULL){
        hal_uart_enable_rxne(Serial2.pUART, false);
        return;
    }
    xStreamBufferSendFromISR(*Serial2.pBuffer, &rxData, sizeof(uint8_t), &xHigherPriorityTaskWoken);

}

void UART4_IRQHandler(void){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t rxData = hal_uart_read_byte(Serial4.pUART);
    // Null check stream buffer (disable IRQ and return on failure)
    if(Serial4.pBuffer == NULL){
        hal_uart_enable_rxne(Serial4.pUART, false);
        return;
    }
    xStreamBufferSendFromISR(*Serial4.pBuffer, &rxData, sizeof(uint8_t), &xHigherPriorityTaskWoken);

}
