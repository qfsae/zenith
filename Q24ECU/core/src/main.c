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
#include "interfaces/interface_uart.h"
#include "interfaces/interface_can.h"
#include "interfaces/interface_sysError.h"
#include "interfaces/interface_adc.h"
#include "stm32f446xx.h"
#include "tasks/runner.h"

TaskHandle_t xRunnerTaskHandle;

StaticTask_t xRunnerTaskBuffer;

StackType_t xRunnerTaskStack[configMINIMAL_STACK_SIZE];

int main(void){

    // Initialize Timer Interrupts

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    // TIM_basic_Init(TIM6, 45000U, 2000U);
    // NVIC_SetPriority(TIM6_DAC_IRQn, NVIC_Priority_MIN); // Enable Timer IRQ (lowest priority)
    // NVIC_EnableIRQ(TIM6_DAC_IRQn);
    
    // Initialize OS Interfaces

    os_uart_setup();

    // clear terminal
    printf("\033[2J");
    printf("\n");
    printf("USART Initialized..\n");

    os_sysError_setup();
    printf("System Error Handler Initialized..\n");

    os_can_setup();
    printf("CAN Bus Initialized..\n");

    os_adc_setup();
    printf("ADC Initialized..\n");

    spin(9999999UL);

    printf("System Starting Runner Task...\n\n");

    xRunnerTaskHandle = xTaskCreateStatic(
        vTask_Runner,
        "Runner",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY+10,
        xRunnerTaskStack,
        &xRunnerTaskBuffer
    );

    spin(9999999UL);  

    // Initialize all the tasks
    // os_task_init();

    // Print out Task Information


    printf("\n");
    printf("Total Heap Memory: %d B\n", configTOTAL_HEAP_SIZE);
    printf("Free Heap Memory:  %d B\n", xPortGetFreeHeapSize());
    printf("\n");


    printf("Starting Scheduler...\n");
    printf("\n");

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        // gpio_write(debug_led2, false);
        __asm__("nop");
    }
    return 0;
}

