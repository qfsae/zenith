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

    //adc
    // ADC1->CR2 |= ADC_CR2_ADON;
    // ADC1->CR2 |= ADC_CR2_CONT;
    // ADC1->CR2 |= ADC_CR2_SWSTART;

    uint16_t led1 = PIN('B', 0);
    uint16_t led2 = PIN('B', 1);
    uint16_t tx = PIN('A', 2), rx = PIN('A', 3); // configure using macros
    //SysTick_Config(FREQ / 1000);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_set_mode(tx, GPIO_MODE_ANALOG); // setup pin as input
    gpio_set_mode(rx, GPIO_MODE_OUTPUT);
    //uart_init(UART_DEBUG, 9600);
    volatile uint32_t timer = 0, period = 100;
    gpio_write(rx, true);
    //gpio_write(led2, true);
    //gpio_write(rx, true);
    //gpio_pull(tx, GPIO_PULLUP); //
    s_ticks = 0x0;
    //adc_init_single(ADC1);

    // printf("Running STM32Core\t...\n");
    // printf("SystemCoreClock: %ld\n", SystemCoreClock);
    //GPIOA->PUPDR &= 3U << 2*2; // reset A2 pullup pulldown register
    //GPIOA->PUPDR |= 1U << 2*2; // set A2 -> pullup

    //adc
    RCC->APB2ENR |= BIT(8);
    ADC1->CR2 &= ~(ADC_CR2_CONT);
    ADC1->SQR1 &= ~(ADC_SQR1_L);
    ADC1->SQR3 |= ADC_SQR3_SQ1;
    ADC1->SMPR1 &= 0x0UL;
    ADC1->SMPR2 &= 0x0UL;
    

    for(;;) {
        //bool tx_on = gpio_read_idr(tx);//GPIOA->IDR & 1U << 2; // read GPIO A2 input data register
        //gpio_write(led2, tx_on);
        if(timer_expired(&timer, period, s_ticks)){
            ADC1->CR2 |= ADC_CR2_SWSTART;
            while(ADC1->SR & ADC_SR_EOC == 0) asm("nop");
            uint16_t adcread = ADC1->DR; //adc_read_single(ADC1, tx);
            if(adcread >0){
                gpio_write(led2, true);
            }
            else{
                gpio_write(led2, false);
            }
            gpio_write(led1, led_on);
            led_on = !led_on;
            //gpio_pull(tx, GPIO_RESET);
            //printf("LED: %d, Ticks: %lu\r\n", led_on, s_ticks);//bricks
            //uart_write_buf(UART2, "HI\n", 4);
        }
    }
    return 0;
}