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
#include "drivers/can.h"
#include "drivers/adc.h"
#include "drivers/uart.h"
#include "stm32f446xx.h"
#include "core.h"

TaskHandle_t xCoreTaskHandle;

StaticTask_t xCoreTaskBuffer;
#define CoreTaskStackSize 0x200
StackType_t xCoreTaskStack[CoreTaskStackSize];

int main(void){
    // Initialize the Serial Interface (Baud = 9600 default)
    uart_init(9600);

    // clear terminal
    printf("\033[2J");
    printf("\n");

    // Initialize System Interfaces
    printf("USART Initialized..\n");
    printf("Initializing ADC..\n");
    adc_init();
    printf("ADC Initialized\n");
    printf("Initializing CAN Bus..\n");
    can_init();
    printf("CAN Bus Initialized\n");
    printf("Starting Core Task..\n");

    xCoreTaskHandle = xTaskCreateStatic(
        vTask_Core,
        "Core",
        CoreTaskStackSize,
        NULL,
        tskIDLE_PRIORITY+10,
        xCoreTaskStack,
        &xCoreTaskBuffer
    );

    printf("Core Task Initialized\n");

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

