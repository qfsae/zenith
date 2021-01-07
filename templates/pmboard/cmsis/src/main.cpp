#include "stm32f446xx.h"
#include <math.h>

#define LED_PORT GPIOB
#define LED_DDR GPIOB
#define LED 1

void delay() {
    for (volatile long long i = 0; i < 500000; i++);
}

int main(void) {
    SystemCoreClockUpdate();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // enable GPIO port B  clock
    LED_PORT->MODER |= LED; // set bit 0 to output
    LED_PORT->ODR = 0;
    for (;;) {
        LED_PORT->ODR ^= LED;
        delay();
    }
}