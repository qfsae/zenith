/**
 * @file main.c
 * @author Jacob Chisholm (https://jchisholm.github.io) //
 * @brief System Entry Point
 * @date 2023-08-30
 * @version 2.2
 * 
 */

#include "main.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "hal/hal_adc.h"
#include <string.h>
// #include <hal/hal_uart.h>
#include "hal/hal_flash.h"
#include "interfaces/usart.h"

// set up a basic test task
TaskHandle_t gTestTask = NULL;

// test task for blinking an led
void testTask(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        // Toggle LED Pin
        gpio_toggle_pin(debug_led2);
        printf("%d\n", xTaskGetTickCount());
        vTaskDelay(1000);
    }
}

// Task Handle for Debug print Task
TaskHandle_t gDebugPrint = NULL;

// Debug Print "HI!" Task
void debugPrint(void *param){
    (void)param;
    for(;;){
        printf("%d\n", xTaskGetTickCount());
        vTaskDelay(1000);
    }
}

void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(debug_led1);
}

USART_t debug;

int main(void){
    // set up gpio
    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    gpio_write(debug_led1, false);
    gpio_set_mode(debug_led2, GPIO_MODE_OUTPUT);
    gpio_write(debug_led2, true);
    gpio_pull(debug_led1, GPIO_PULLDOWN);

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03); // Enable Timer IRQ (lowest priority)
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // hal_uart_init(UART_DEBUG, 9600);
    usart_send_init(&debug, UART_DEBUG, 38400);
    // xSemaphoreGive(debug.handle);
    printf("system starting tasks...\n");

    // Create Sample Blink Task
    uint32_t status_tstTsk = xTaskCreate(
        testTask,
        "TestTsk",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &gTestTask
    );

    // Create Sample Print Task
    uint32_t status_dbgPrn = xTaskCreate(
        debugPrint,
        "DebugPrint",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &gDebugPrint
    );

    // xTaskGetHandle()

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        gpio_write(debug_led2, false);
    }
    return 0;
}