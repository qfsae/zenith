#include "hal/usart2.h"

volatile FIFO8_64 USART_RX;
volatile FIFO8_64 USART_TX;

// Triggered on all USART interrupts -> see STM32F446 Manual
void USART2_IRQHandler(){
    //USART2->SR &= ~(USART_SR_TXE);
    if(USART_TX.length > 0){
        USART2->DR = FIFO8_pop(&USART_TX);
        //uart_write_byte(UART_DEBUG, FIFO8_pop(&USART_RX));
        //USART_RX.length--;
        if(USART_TX.length == 0) uart_enable_txne(USART2, false);
    }
    
    if(uart_read_ready(USART2)){
        uart_enable_txne(USART2, true);
        FIFO8_push(&USART_RX, uart_read_byte(USART2));
    }
}

void USART2_Init(unsigned long baud){
    uart_init(USART2, baud);
    uart_enable_rxne(USART2, true);
    NVIC_SetPriority(USART2_IRQn, 0x03);
    NVIC_EnableIRQ(USART2_IRQn);
}

uint8_t USART2_Transmit(volatile uint8_t data){
    FIFO8_push(&USART_TX, data);
    uart_enable_txne(USART2, true);
    return (USART_TX.length >= 64);
}

uint8_t USART2_Transmit_Buffer(char *buf, size_t len){
    //FIFO8_push_buf(&USART_TX, buf, len);
    while(len-- > 0) FIFO8_push(&USART_TX, *(uint8_t *) buf++);
    uart_enable_txne(USART2, true);
    return (USART_TX.length >= 64);
}

uint8_t USART2_Receive(){
    return FIFO8_pop(&USART_RX);
}

uint8_t USART2_RXBuf_Len(){
    return USART_RX.length;
}