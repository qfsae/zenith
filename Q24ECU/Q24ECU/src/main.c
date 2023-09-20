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
#include "hal_adc.h"

static volatile uint32_t s_ticks = 0xBEEF;
void SysTick_Handler(void){
    s_ticks++;
}

volatile bool led_on = true;
volatile uint64_t increment = 0xDEADBEEF;


void initSystem(void){
    clock_init();
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(SYS_FREQUENCY/1000); // DO NOT USE "SystemClock"
    increment = 0x0;
    s_ticks = 0x0;
    adc_init(ADC2);
    SystemCoreClockUpdate();

}

int main(void){
    initSystem();

    uint16_t led1 = PIN('B', 0);
    uint16_t led2 = PIN('B', 1);
    uint16_t current = PIN('C', 0);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(current, GPIO_MODE_ANALOG);
    gpio_set_mode(PIN('C', 1), GPIO_MODE_ANALOG);
    uart_init(UART_DEBUG, 9600);
    volatile uint32_t timer = 0, period = 1000;

    for(;;) { // while loop that pulls from que | que added to by interrupts
        if(timer_expired(&timer, period, s_ticks)){
            if(!led_on){
                gpio_write(led2, true);
            }
            else{
                gpio_write(led2, false);
            }
            gpio_toggle_pin(led1);
            led_on = !led_on;
            SystemCoreClockUpdate();
            //printf("SYSTick: %ld\tCore: %ld\tFreq: %ld\t", s_ticks, SystemCoreClock, SYS_FREQUENCY);
            printf("VBatt: %0.3fV\n", 13.5*adc_poll(ADC2, 11)/4096.0);
            //printf("source: %d\tm: %d\tn: %d\tp: %d\tCore: %ld\n", (RCC->CFGR & RCC_CFGR_SWS), RCC->PLLCFGR & RCC_PLLCFGR_PLLM, ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6), (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2, SystemCoreClock);
            //uart_write_buf(USART2, "hi\n", 4);
            //printf("%f\t", adc_poll(ADC2, 10)*3.3*1.6/(4096));
            // printf("%d\n", adc_poll(ADC2, 10));
            // ADC1->CR2 |= ADC_CR2_ADON;W
            //printf("SR: %ld\t", ADC2->SR);
            //printf("LED: %d, Ticks: %lu\r\n", led_on, s_ticks);
        }
        //gpio_toggle_pin(led1);
    }
    return 0;
}