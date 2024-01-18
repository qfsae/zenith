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

#define UART_WRITE_OK 0
#define UART_ERR_UNDEF 1
#define UART_ERR_ACC 2


/**
 * @brief UART port typedef
 * @param port The UART port (CMSIS UART typedef)
 * @param semaphore The semaphore controlling the UART port
 * @param rxbuffer The receive buffer loaded by the UART IRQ Handler
 * 
 */
typedef struct uart_t {
    USART_TypeDef *port;
    xSemaphoreHandle semaphore;
    StreamBufferHandle_t rxbuffer;
} uart_t;

// UART OS Handlers
extern uart_t port_uart2;

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
extern void os_uart_setup(void);


/**
 * @brief Initialize a USART device and its Semaphore
 * 
 * @param port The USART_t typedef handle
 * @param uart the USARTx define from CMSIS headers
 * @param baud USART Baud Rate
 */
static inline void uart_send_init(uart_t *port, USART_TypeDef *uart, unsigned long baud){
    port->semaphore = xSemaphoreCreateBinary();
    if(port->semaphore == NULL) return;
    port->rxbuffer = xStreamBufferCreate(64, 1);
    if(port->rxbuffer == NULL) return;
    port->port = uart;
    if(port->port == NULL) return;
    hal_uart_init(port->port, baud);
    xSemaphoreGive(port->semaphore);
}


// USART2 IQR Handler
extern void USART2_IRQHandler(void);

/**
 * @brief Task Blocking command to send a byte over uart
 * 
 * @param port The uart_t port to use (Must be initialized)
 * @param byte The Byte to transmit
 * @param timeout The amount of ticks to wait for the interface to become available
 */
static inline int uart_send_blocking(uart_t *port, uint8_t byte, TickType_t timeout){
    if(port == NULL)            return UART_ERR_UNDEF;
    if(port->port == NULL)      return UART_ERR_UNDEF;
    if(port->semaphore == NULL) return UART_ERR_UNDEF;
    if(xSemaphoreTake(port->semaphore, timeout) == pdTRUE){
        hal_uart_write_byte(port->port, byte);
        xSemaphoreGive(port->semaphore);
        return UART_WRITE_OK;
    }
    return UART_ERR_ACC;
}

/**
 * @brief Task Blocking command to send a buffer over uart
 * 
 * @param port The uart_t port to use (Must be initialized)
 * @param buf Data Buffer
 * @param len Length of Data Buffer
 * @param timeout The amount of ticks to wait for the interface to become available 
 */
static inline int uart_send_buf_blocking(uart_t *port, char* buf, size_t len, TickType_t timeout){
    if(port == NULL)            return UART_ERR_UNDEF;
    if(port->port == NULL)      return UART_ERR_UNDEF;
    if(port->semaphore == NULL) return UART_ERR_UNDEF;
    if(xSemaphoreTake(port->semaphore, timeout) == pdTRUE){
        hal_uart_write_buf(port->port, buf, len);
        xSemaphoreGive(port->semaphore);
        return UART_WRITE_OK;
    }
    return UART_ERR_ACC;
}

