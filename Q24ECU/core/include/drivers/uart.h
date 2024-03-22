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


typedef struct UART_StreamBuffer {
    char buffer[64];
    StaticStreamBuffer_t staticBuffer;
    StreamBufferHandle_t handle;
} UART_StreamBuffer_t;


/**
 * @brief UART port typedef
 * @param port The UART port (CMSIS UART typedef)
 * @param writeHandler The semaphore controlling write access to the UART port
 * @param readHandler The semaphore controlling read access to the UART receive buffer
 * @param pbuffer Pointer to the receive buffer loaded by the UART IRQ Handler
 * 
 */
typedef struct UART_Handle {
    USART_TypeDef *pUART;
    StaticSemaphore_t writeSemaphore;
    xSemaphoreHandle writeHandler;
    StaticSemaphore_t readSemaphore;
    xSemaphoreHandle readHandler;
    StreamBufferHandle_t *pBuffer;
} UART_Handle_t;

extern UART_Handle_t Serial2, Serial4;


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
 * @param pHandle Pointer to the UART Handle to open
 * @param stream A pointer to preallocated memory for the stream buffer
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_open_buffer(UART_Handle_t *pHandle, UART_StreamBuffer_t *stream, TickType_t timeout);

/**
 * @brief Close a UART RX Buffer (disables RX Interrupts)
 * 
 * @param pHandle Pointer to the Handle to close
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_close_buffer(UART_Handle_t *pHandle);

/**
 * @brief Write a byte to a UART port
 * 
 * @param pHandle Pointer to the Handle for the port to write to
 * @param byte byte to write
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_write_byte(UART_Handle_t *pHandle, char byte, TickType_t timeout);

/**
 * @brief Write a buffer to a UART port
 * 
 * @param pHandle Pointer to the Handle for the port to write to
 * @param buf A pointer to the buffer to write
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_write(UART_Handle_t *pHandle, char* buf, size_t len, TickType_t timeout);

/**
 * @brief Write a String to a uart port
 * 
 * @param pHandle Pointer to the Handle for the port to write to
 * @param str The String to write
 * @param timeout The amount of ticks to wait for the interface to become available
 * @return SYS_OK if successful
 */
extern enum SYS_ERROR uart_write_str(UART_Handle_t *pHandle, char* str,  TickType_t timeout);


/* All IRQ Handlers must be globally accessible for linking purposes */

// USART2 IQR Handler
extern void USART2_IRQHandler(void);

// USART2 IQR Handler
extern void UART4_IRQHandler(void);

