/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * from bare metal programming guide by --
 * @brief simple led blinking program
 * @date 2023-08-30
 * 
 */

#include "hal/hal_gpio.h"
#include "hal/hal_uart.h"
#include "hal/hal_adc.h"

#include "fifo.h"

uint16_t led1 = PIN('B', 0);
uint16_t led2 = PIN('B', 1);

static volatile uint32_t s_ticks = 0xBEEF;
void SysTick_Handler(void){
    s_ticks++;
}

volatile bool led_on = true;
volatile uint64_t increment = 0xDEADBEEF;

volatile FIFO8_64 USART_RX;
volatile FIFO8_64 USART_TX;


void initSystem(void){
    clock_init();
    RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(SYS_FREQUENCY/1000); // DO NOT USE "SystemClock"
    increment = 0x0;
    s_ticks = 0x0;
    adc_init(ADC2);
    //uart_enable_rxne(UART_DEBUG, true);
    SystemCoreClockUpdate();

}

volatile uint8_t interfaceCMD = 0, cmdData[4];
volatile uint8_t cmdRecv = 0;
volatile uint8_t leng = 1;

// Triggered on all USART interrupts -> see STM32F446 Manual
void USART2_IRQHandler(){
    //USART2->SR &= ~(USART_SR_TXE);
    if(USART_RX.length > 0){
        USART2->DR = FIFO8_pop(&USART_RX);
        //uart_write_byte(UART_DEBUG, FIFO8_pop(&USART_RX));
        USART_RX.length--;
        if(USART_RX.length == 0) uart_enable_txne(USART2, false);
    }
    
    if(uart_read_ready(USART2)){
        FIFO8_push(&USART_RX, uart_read_byte(USART2));
        uart_enable_txne(USART2, true);
        USART_RX.length++;
    }
    gpio_toggle_pin(led1);
    
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
    //gpio_toggle_pin(led1);
    //if((TIM6->SR & 1) == 1) gpio_toggle_pin(led1);
    // TIM6->ARR = (65534);
}

int main(void){
    initSystem();
    FIFO8_init(&USART_TX);
    FIFO8_init(&USART_RX);
    
    uint16_t current = PIN('C', 0);
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_write(led1, false);
    gpio_set_mode(current, GPIO_MODE_ANALOG);
    gpio_set_mode(PIN('C', 1), GPIO_MODE_ANALOG);

    // Initialize USART
    uart_init(UART_DEBUG, 9600);
    // Enable USART receive interrupt
    uart_enable_rxne(UART_DEBUG, true);
    //uart_enable_txne(UART_DEBUG, true);
    
    // Set up ARM interrupt (0x03 = lowest priority | 0x00 = highest priority)
    NVIC_SetPriority(USART2_IRQn, 0x03);
    NVIC_EnableIRQ(USART2_IRQn);

    volatile uint32_t timer = 0, period = 1000;

    // Enable Timer 6 (Basic Timer) 10Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 200U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    for(;;) { // while loop that pulls from que | que added to by interrupts
            // if(USART_TX.length != 0){
            //     //USART2->DR = FIFO8_pop(&USART_TX);
            //     uart_write_byte(UART_DEBUG, FIFO8_pop(&USART_TX));
            // }
        if(timer_expired(&timer, period, s_ticks)){
            // if(!led_on){
            //     gpio_write(led2, true);
            // }
            // else{
            //     gpio_write(led2, false);
            // }
            //printf("\nLength: %d\n", leng);
            gpio_toggle_pin(led2);
            // FIFO8_push(&USART_TX, 'h');
            // FIFO8_push(&USART_TX, 'e');
            // FIFO8_push(&USART_TX, 'l');
            // FIFO8_push(&USART_TX, 'l');
            // FIFO8_push(&USART_TX, 'o');
            // FIFO8_push(&USART_TX, '\n');
            
            // led_on = !led_on;
            //printf("%d\n", (TIM6->CNT & 0xFFFF));
            //printf("Command: %d Data: %c %c %c %c\n", interfaceCMD, cmdData[0], cmdData[1], cmdData[2], cmdData[3]);
        }
        //gpio_toggle_pin(led1);
    }
    return 0;
}