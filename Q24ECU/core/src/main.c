/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */

#include "main.h"
#include "limits.h"
#include "taskHandlers.h"

// Init global handles for tasks

TaskHandle_t tskh_Test1 = NULL;
TaskHandle_t tskh_BlinkLED = NULL;
TaskHandle_t tskh_USART2_Handler = NULL;

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

    // hal_uart_init(UART_DEBUG, 9600);
    uart_setup();
    // xSemaphoreGive(debug.handle);
    printf("\nsystem starting tasks...\n");

    // Create Sample Blink Task
    xTaskCreate(
        tsk_BlinkLED,
        "blink",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_BlinkLED
    );

    // Create Sample Print Task
    xTaskCreate(
        tsk_Test1,
        "tst1",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_Test1
    );
    
    // Create UART2 (debug) receive task handler
    xTaskCreate(
        tsk_USART2_Handler,
        "u2",
        1024,
        NULL,
        tskIDLE_PRIORITY+4,
        &tskh_USART2_Handler
    );

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        gpio_write(debug_led2, false);
    }
    return 0;
}