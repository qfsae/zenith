/**
 * @file uart.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief UART Driver (For FreeRTOS based system)
 * @version 1.2
 * @date 2024-03-04
 * 
 * Version 1.0
 * - Created UART Interface code
 * Version 1.1
 * - Added RX Interrupt + Handler
 * Version 1.2
 * - Refactored as a driver (No task + buffer handling done outside of driver)
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "hal/hal_uart.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "stream_buffer.h"
#include "errors.h"


struct uart_streamBuffer {
    char buffer[64];
    StaticStreamBuffer_t staticBuffer;
    StreamBufferHandle_t handle;
};


/**
 * @brief UART port typedef
 * @param port The UART port (CMSIS UART typedef)
 * @param writeHandler The semaphore controlling write access to the UART port
 * @param readHandler The semaphore controlling read access to the UART receive buffer
 * @param pbuffer Pointer to the receive buffer loaded by the UART IRQ Handler
 * 
 */
struct uart_port {
    USART_TypeDef *port;
    StaticSemaphore_t writeSemaphore;
    xSemaphoreHandle writeHandler;
    StaticSemaphore_t readSemaphore;
    xSemaphoreHandle readHandler;
    StreamBufferHandle_t *pBuffer;
};

extern struct uart_port port_uart2, port_uart4;

/**
 * @brief UART Initialization Handler
 * 
 * This function will initialize all onboard UART interfaces.
 * This function should not be called more than once.
 * 
 */
extern void uart_init(unsigned long baud);

/**
 * @brief Open a UART RX Buffer (enables RX Interrupts)
 * @note Buffer Handling must be done inside the calling function
 * @param port The UART Port to open
 * @param stream A pointer to preallocated memory for the stream buffer
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_open_buffer(struct uart_port *port, struct uart_streamBuffer *stream, TickType_t timeout);

/**
 * @brief Close a UART RX Buffer (disables RX Interrupts)
 * 
 * @param port The port to close
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_close_buffer(struct uart_port *port);

/**
 * @brief Write a byte to a UART port
 * 
 * @param port The port to write to
 * @param byte byte to write
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_write_byte(struct uart_port *port, char byte, TickType_t timeout);

/**
 * @brief Write a buffer to a UART port
 * 
 * @param port The port to write to
 * @param buf A pointer to the buffer to write
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_write(struct uart_port *port, char* buf, size_t len, TickType_t timeout);

/* All IRQ Handlers must be globally accessible for linking purposes */

// USART2 IQR Handler
extern void USART2_IRQHandler(void);

