/**
 * @file interface_uart.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief uart FreeRTOS Interface with Semaphore Handler
 * @version 0.1
 * @date 2023-11-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hal/hal_uart.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"

typedef struct {
    USART_TypeDef *interface;
    xSemaphoreHandle handle;
    StreamBufferHandle_t stream;
} uart_t;

// handler for DEBUG USART (USART 2 on MockECU)
extern uart_t debug;

extern void uart_setup();

// USART2 IQR Handler
extern void USART2_IRQHandler();


/**
 * @brief Initialize a USART device and its Semaphore
 * 
 * @param handle The USART_t typedef handle
 * @param interface the USARTx define from CMSIS headers
 * @param baud USART Baud Rate
 */
static inline void uart_send_init(uart_t *handle, USART_TypeDef *interface, unsigned long baud){
    handle->handle = xSemaphoreCreateBinary();
    handle->stream = xStreamBufferCreate(64, 1);
    if(handle->handle == NULL) return;
    handle->interface = interface;
    hal_uart_init(handle->interface, baud);
    xSemaphoreGive(handle->handle);
}

/**
 * @brief Task Blocking command to send a byte over uart
 * 
 * @param port The uart_t port to use (Must be initialized)
 * @param byte The Byte to transmit
 * @param timeout The amount of ticks to wait for the interface to become available
 */
static inline void uart_send_blocking(uart_t *port, uint8_t byte, TickType_t timeout){
    if(port == NULL)              return;
    if(port->interface == NULL)   return;
    if(port->handle == NULL)      return;
    if(xSemaphoreTake(port->handle, timeout) == pdTRUE){
        hal_uart_write_byte(port->interface, byte);
        xSemaphoreGive(port->handle);
    }
}

/**
 * @brief 
 * 
 * @param port The uart_t port to use (Must be initialized)
 * @param buf Data Buffer
 * @param len Length of Data Buffer
 * @param timeout The amount of ticks to wait for the interface to become available 
 */
static inline void uart_send_buf_blocking(uart_t *port, char* buf, size_t len, TickType_t timeout){
    if(port == NULL)              return;
    if(port->interface == NULL)   return;
    if(port->handle == NULL)      return;
    if(xSemaphoreTake(port->handle, timeout) == pdTRUE){
        hal_uart_write_buf(port->interface, buf, len);
        xSemaphoreGive(port->handle);
    }
}