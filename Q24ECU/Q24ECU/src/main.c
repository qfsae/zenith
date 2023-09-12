/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * from bare metal programming guide by --
 * @brief simple led blinking program
 * @date 2023-08-30
 * 
 */

#include "hal_gpio.h"
#include "hal_uart.h"

static volatile uint32_t s_ticks = 0xBEEF;
void SysTick_Handler(void){
    s_ticks++;
}

volatile bool led_on = true;
volatile uint64_t increment = 0xDEADBEEF;

int main(void){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(SystemCoreClock/1000);
    uint16_t led1 = PIN('B', 0);
    uint16_t led2 = PIN('B', 1);
    //SysTick_Config(FREQ / 1000);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    uart_init(USART2, 9600);
    volatile uint32_t timer = 0, period = 100;
    gpio_write(led2, true);
    s_ticks = 0x0;
    printf("Running STM32Core\t...\n");
    printf("SystemCoreClock: %ld\n", SystemCoreClock);
    for(;;) {
        if(timer_expired(&timer, period, s_ticks)){
            gpio_write(led1, led_on);
            led_on = !led_on;
            printf("LED: %d, Ticks: %lu\r\n", led_on, s_ticks);//bricks
            //uart_write_buf(UART2, "HI\n", 4);
        }
    }
    return 0;
}