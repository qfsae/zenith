#pragma once
/**
 * @file fifo.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Basic FIFO Implementation (Does not consider overflow)
 * @version 0.1
 * @date 2023-10-20
 * 
 * @copyright Copyright (c) 2023
 * Does NOT consider overflow
 */
#include <inttypes.h>

// 64 bit FIFO consisting of 8 bit values
typedef volatile struct {
    uint8_t data[64];
    uint8_t head, tail;
    uint8_t length;
} FIFO8_64;

/**
 * @brief Pop the top element from the FIFO
 * 
 * @param reg data register
 * @return uint8_t 
 */
uint8_t FIFO8_pop(volatile FIFO8_64 *reg);

/**
 * @brief push an element onto the bottom of the stack
 * 
 * @param reg 
 * @return uint8_t 
 */
uint8_t FIFO8_push(volatile FIFO8_64 *reg, uint8_t data);

uint8_t FIFO8_init(volatile FIFO8_64 *reg);