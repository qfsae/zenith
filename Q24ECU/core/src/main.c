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
#include "stm32f446xx.h"

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

    os_can_setup();
    printf("CAN Bus Initialized..\n");

    spin(9999999UL);

    printf("System Starting Tasks...\n\n");

    spin(9999999UL);  

    // Initialize all the tasks
    os_task_init();

    // Print out Task Information
    printf("Task:\t   Identifier\t    Priority\t\tState\n");
    for (int i = 0; i < eTask_TaskCount; i++)
    {
        printf("%d: %16s\t\t%lu\t\t", i, pcTaskGetName(xTaskHandles[i]), uxTaskPriorityGet(xTaskHandles[i]));
        switch(eTaskGetState(xTaskHandles[i])) {
            case eRunning:
                printf("RUNNING\n");
                break;
            case eReady:
                printf("READY\n");
                break;
            case eBlocked:
                printf("BLOCKED\n");
                break;
            case eSuspended:    
                printf("SUSPENDED\n");
                break;
            case eDeleted:   
                printf("DELETED\n");
                break;
            case eInvalid:   
                printf("INVALID\n");
                break;
            default:
                printf("UNKNOWN\n");
                break;
        }
    }

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

