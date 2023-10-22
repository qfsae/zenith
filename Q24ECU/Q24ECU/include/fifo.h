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
#include <stdlib.h>

// 64 bit FIFO consisting of 8 bit values
typedef volatile struct {
    uint8_t data[64];
    uint8_t head, tail;
    uint8_t length;
} FIFO8_64;

static inline uint8_t FIFO8_pop(volatile FIFO8_64 *reg){
    //if((reg->head++) == 64) reg->head = 0;
    reg->head = (reg->head + 1) % 64;
    reg->length--;
    return reg->data[reg->head];
}

static inline uint8_t FIFO8_push(volatile FIFO8_64 *reg, uint8_t data){
    //if((reg->tail + 1) == reg->head) return 1;
    // if(reg->tail == 64) reg->tail = 0;
    reg->tail = (reg->tail + 1) % 64;
    reg->length++;
    reg->data[reg->tail] = data;
    if(reg->head == -1) reg->head = reg->tail;
    return 0;
}

static inline uint8_t FIFO8_init(volatile FIFO8_64 *reg){
    for (int i = 0; i < 64; i++)
    {
        reg->data[i] = 0;
    }
    reg->head = -1;
    reg->tail = -1;
    reg->length = 0;
}

// static inline void FIFO8_push_buf(FIFO8_64 *reg, char *buf, size_t len){
//     while(len-- > 0) FIFO8_push(reg, *(uint8_t *) buf++);
//     reg->length += len;
// }