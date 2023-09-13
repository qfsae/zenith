/**
 * @file hal_gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stm32f446xx.h>
#include "hal_gpio.h"

#define UART_DEBUG USART2

static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
    // figure 19. selecting an alternate function (7=spi2/3, usart1..3, uart5, spdif-in)
    uint8_t af = 7;           // Alternate function
    uint16_t rx = 0, tx = 0;  // pins

    if (uart == USART1) RCC->APB2ENR |= BIT(4);
    if (uart == USART2) RCC->APB1ENR |= BIT(17);
    if (uart == USART3) RCC->APB1ENR |= BIT(18);

    if (uart == USART1) tx = PIN('A', 9), rx = PIN('A', 10);
    if (uart == USART2) tx = PIN('A', 2), rx = PIN('A', 3);
    if (uart == USART3) tx = PIN('D', 8), rx = PIN('D', 9);

    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, af);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    uart->CR1 = 0;                           // Disable this UART
    uart->BRR = FREQ / baud;                 // FREQ is a UART bus frequency
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3);  // Set UE, RE, TE
}

static inline int uart_read_ready(USART_TypeDef *uart){
    return uart->SR & BIT(5);
}

static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
    return ((uint8_t) (uart->DR & 255));
}

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

static inline void uart_write_byte(USART_TypeDef * uart, uint8_t byte) {
    uart->DR = byte;
    while ((uart->SR & BIT(7)) == 0) spin(1);
    
}

static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len){
    while(len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}