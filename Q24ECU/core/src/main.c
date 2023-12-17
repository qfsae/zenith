/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */

#include "main.h"
#include <stdio.h>
#include "hal/hal_gpio.h"
#include "hal/hal_tim_basic.h"
#include "hal/hal_uart.h"
#include "hal/hal_can.h"
#include "interfaces/interface_uart.h"
#include "stm32f446xx.h"

int main(void){
    // set up gpio
    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    gpio_write(debug_led1, false);
    gpio_set_mode(debug_led2, GPIO_MODE_OUTPUT);
    gpio_write(debug_led2, true);
    gpio_pull(debug_led1, GPIO_PULLDOWN);

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, NVIC_Priority_MIN); // Enable Timer IRQ (lowest priority)
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // initialize os interfaces
    os_uart_setup();
    gpio_set_mode(PIN('A', 11), GPIO_MODE_AF);
    gpio_set_af(PIN('A', 11), 9);
    gpio_set_mode(PIN('A', 12), GPIO_MODE_AF);
    gpio_set_af(PIN('A', 12), 9);
    hal_can_init(CAN1, CAN_1000KBPS);

    // clear terminal
    printf("\033[2J");
    spin(9999999UL);
    printf("system starting tasks...\n");
    spin(9999999UL);  

    // Initialize all the tasks
    os_task_init();

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        gpio_write(debug_led2, false);
    }
    return 0;
}

