/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * from bare metal programming guide by --
 * @brief simple led blinking program
 * @date 2023-08-30
 * 
 */

#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_uart.h"

uint16_t led1 = PIN('B', 0);
uint16_t led2 = PIN('B', 1);

static volatile uint32_t s_ticks = 0xBEEF;
void SysTick_Handler(void){
    s_ticks++;
}

/**
 * @brief Init function for basic timers (TIM6/TIM7)
 * 
 * @param timer TIM6 or TIM6 (STM32F446)
 * @param prescaler Clock Prescaler
 * @param reload_register Overflow Evaluator
 * Note: Values loaded into prescaler and reload are (value-1) -> see datasheet
 */
void TIM_basic_Init(TIM_TypeDef *timer, uint16_t prescaler, uint16_t reload_register){
    // Enable Clock
    if(timer == TIM6) RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    if(timer == TIM7) RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    // Reset and Set Master Mode to Enable (CNT_EN is used as a trigger output)
    timer->CR2 &= ~(7UL << 4U);
    timer->CR2 |= BIT(4); // set timer master mode to enable
    timer->DIER &= ~(BIT(8) | BIT(0)); // reset interrupt enable reg
    timer->DIER |= BIT(0); // enable timer interrupt
    // TIM6->EGR |= BIT(0);
    timer->ARR = (reload_register-1);//(65536);
    timer->PSC = (uint16_t)(prescaler-1);//(APB1_FREQUENCY);
    timer->CR1 &= ~(TIM_CR1_ARPE | TIM_CR1_OPM | TIM_CR1_URS | TIM_CR1_CEN); // reset timer control register 1
    timer->CR1 |= (TIM_CR1_CEN); // set interrupt source to only overflow | enable timer
}

void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;

    //printf("%d\n", TIM6->SR);
    gpio_toggle_pin(led1);
    //if((TIM6->SR & 1) == 1) gpio_toggle_pin(led1);
    // TIM6->ARR = (65534);
}

volatile bool led_on = true;
volatile uint64_t increment = 0xDEADBEEF;


void SystemInit(void){
    clock_init();
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(SYS_FREQUENCY/1000); // DO NOT USE "SystemClock"
    increment = 0x0;
    s_ticks = 0x0;
    adc_init(ADC2);
    // SystemCoreClockUpdate();
}

int main(void){
    uint16_t current = PIN('C', 0);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(current, GPIO_MODE_ANALOG);
    gpio_set_mode(PIN('C', 1), GPIO_MODE_ANALOG);

    // Initialize USART
    uart_init(USART2, 9600);
    uart_enable_rxne(USART2, true);
    NVIC_SetPriority(USART2_IRQn, 0x03);
    NVIC_EnableIRQ(USART2_IRQn);
    volatile uint32_t timer = 0, period = 1000;

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    for(;;) {
        if(timer_expired(&timer, period, s_ticks)){
            // if(!led_on){
            //     gpio_write(led2, true);
            // }
            // else{
            //     gpio_write(led2, false);
            // }
            gpio_toggle_pin(led2);
            ITM_SendChar('2'); // does not work at moment bc of SWIM being disconnected
            //printf("hi\n");
            printf("%d\n", gpio_read_odr(led2));
        }
        //gpio_toggle_pin(led1);
    }
    return 0;
}