#include "fifo.h"

uint8_t FIFO8_pop(volatile FIFO8_64 *reg){
    //if((reg->head++) == 64) reg->head = 0;
    reg->head = (reg->head + 1) % 64;
    // reg->length--;
    return reg->data[reg->head];
}

uint8_t FIFO8_push(volatile FIFO8_64 *reg, uint8_t data){
    //if((reg->tail + 1) == reg->head) return 1;
    // if(reg->tail == 64) reg->tail = 0;
    reg->tail = (reg->tail + 1) % 64;
    // reg->length++;
    reg->data[reg->tail] = data;
    if(reg->head == -1) reg->head = reg->tail;
    return 0;
}

uint8_t FIFO8_init(volatile FIFO8_64 *reg){
    for (int i = 0; i < 64; i++)
    {
        reg->data[i] = 0;
    }
    reg->head = -1;
    reg->tail = -1;
    reg->length = 0;
}