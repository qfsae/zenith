#pragma once

#include "fifo.h"
#include "hal/hal_uart.h"

void    USART2_Init(unsigned long baud);

uint8_t USART2_Transmit(volatile uint8_t data);

uint8_t USART2_Transmit_Buffer(char *buf, size_t len);

uint8_t USART2_Receive();

uint8_t USART2_RXBuf_Len();