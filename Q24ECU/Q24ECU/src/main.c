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
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2)); // enable FPU
    FLASH->ACR |= FLASH_ACR_LATENCY | BIT(8) | BIT(9); // flash latency / prefetch

    // RCC->CR &= ~(BIT(0) | BIT(16) | BIT(18)); // clear hsi on, hse on, hse byp
    // RCC->CR |= BIT(16);


    // RCC->PLLCFGR &= ~((BIT(17) - 1)); // clear pll multipliers
    // RCC->PLLCFGR |= (((PLL_P - 2) / 2) & 3) << 16;      // Set PLL_P 
    // RCC->PLLCFGR |= PLL_M | (PLL_N << 6);               // Set PLL_M and PLL_N 
    // RCC->CR |= BIT(24);                                 // Enable PLL 
    // while ((RCC->CR & BIT(25)) == 0) spin(1);           // Wait until done 
    // RCC->CFGR = (APB1_PRE << 10) | (APB2_PRE << 13);    // Set prescalers 
    // RCC->CFGR |= 2;                                     // Set clock source to PLL 
    // while ((RCC->CFGR & 12) == 0) spin(1);              // Wait until done 

    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
    FLASH->ACR |= FLASH_ACR_LATENCY | BIT(8) | BIT(9);
    SysTick_Config(SystemCoreClock/1000);
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
    volatile uint32_t timer = 0, period = 200;   

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
            //printf("CPU Speed: %ld\n", SystemCoreClock);
            uart_write_buf(USART2, "hi\n", 4);
            // printf("%d\t", adc_read(ADC2, 10));
            // printf("%d\n", adc_read(ADC2, 11));
            // ADC1->CR2 |= ADC_CR2_ADON;W
            //printf("SR: %ld\t", ADC2->SR);
            //printf("LED: %d, Ticks: %lu\r\n", led_on, s_ticks);
        }
    }
    return 0;
}