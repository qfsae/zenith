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
#include "queue.h"
#include "semphr.h"


xSemaphoreHandle usartHandle = NULL;

// set up a basic test task
TaskHandle_t gTestTask = NULL;

// test task for blinking an led
void testTask(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        // Toggle LED Pin
        gpio_toggle_pin(debug_led2);
        if(usartHandle != NULL){
            if(xSemaphoreTake(usartHandle, (TickType_t) 0) == pdTRUE){
                //uart_write_buf(UART_DEBUG, "Task2\n", 7);
                char *name;
                name = pcTaskGetName(NULL);
                printf("Hi from %s\n" name);
                xSemaphoreGive(usartHandle);
            }
        }
        // Non Blocking Delay
        vTaskDelay(1000);
    }
}

// Task Handle for Debug print Task
TaskHandle_t gDebugPrint = NULL;

// Debug Print "HI!" Task
void debugPrint(void *param){
    (void)param;
    //gpio_set_mode(PIN_voltageSensor, GPIO_MODE_ANALOG);
    //printf("Task Start: debugPrint\n");
    // adc_init(ADC2);
    //hal_FLASH_WriteHW(0x08060010, 55UL);
    for(;;){
        //int adcread = *hal_FLASH_Read(0x08060000);//adc_poll(ADC2, 11);
        //printf("Read: %d\n", adcread);
        // uart_write_buf(USART2, "Hi!\n", 5);
        // wait max of 1ms
        if(usartHandle != NULL){
            if(xSemaphoreTake(usartHandle, (TickType_t) 10) == pdTRUE){
                uart_write_buf(UART_DEBUG, "Task1\n", 7);
                xSemaphoreGive(usartHandle);
            }
        }
        //printf("HI!!\n");
        vTaskDelay(1000);
    }
}

void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(debug_led1);
}

int main(void){
    usartHandle = xSemaphoreCreateBinary();
    xSemaphoreGive(usartHandle);

    // set up gpio
    gpio_set_mode(debug_led1, GPIO_MODE_OUTPUT);
    gpio_write(debug_led1, true);
    gpio_set_mode(debug_led2, GPIO_MODE_OUTPUT);
    gpio_write(debug_led2, false);
    gpio_pull(debug_led1, GPIO_PULLDOWN);

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03); // Enable Timer IRQ (lowest priority)
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    uart_init(UART_DEBUG, 9600);

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

    // Start Scheduler
    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        gpio_write(debug_led2, false);
    }
    return 0;
}