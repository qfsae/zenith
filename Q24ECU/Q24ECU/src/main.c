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
    //uart_enable_rxne(UART_DEBUG, true);
    SystemCoreClockUpdate();

}

volatile uint8_t interfaceCMD = 0, cmdData[4];
volatile uint8_t cmdRecv = 0;

// Triggered on all USART interrupts -> see STM32F446 Manual
void USART2_IRQHandler(){
    if(cmdRecv == 0){
        interfaceCMD = uart_read_byte(UART_DEBUG);
        cmdRecv++;
    }
    else if(cmdRecv <= 4){
        cmdData[cmdRecv-1] = uart_read_byte(UART_DEBUG);
        cmdRecv++;
    }
    else{
        cmdRecv = 0;
    }
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

    // Initialize USART
    uart_init(UART_DEBUG, 9600);
    // Enable USART receive interrupt
    uart_enable_rxne(UART_DEBUG, true);
    // Set up ARM interrupt (0x03 = lowest priority | 0x00 = highest priority)
    NVIC_SetPriority(USART2_IRQn, 0x03);
    NVIC_EnableIRQ(USART2_IRQn);

    volatile uint32_t timer = 0, period = 1000;

    for(;;) { // while loop that pulls from que | que added to by interrupts
        if(timer_expired(&timer, period, s_ticks)){
            if(!led_on){
                gpio_write(led2, true);
            }
            else{
                gpio_write(led2, false);
            }
            //gpio_toggle_pin(led1);
            led_on = !led_on;
            printf("Command: %c Data: %c %c %c %c\n", interfaceCMD, cmdData[0], cmdData[1], cmdData[2], cmdData[3]);
        }
        //gpio_toggle_pin(led1);
    }
    return 0;
}