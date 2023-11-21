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

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        // Print out the systemtick timer once a second
        printf("%d\n", xTaskGetTickCount());
        vTaskDelay(1000);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    for(;;){
        // This task is currently not used so perma suspend it
        vTaskSuspend(NULL);
    }
}

void tsk_USART2_Handler(void *param){
    (void)param;
    for(;;){
        uint8_t buf[64];
        size_t bytes = xStreamBufferReceive(debug.stream, (void*) &buf, 64, portMAX_DELAY);
        printf("%s (%d)\n", buf, bytes);
        // reset the stream buffer
        for (int i = 0; i < 64; i++)
        {
            buf[i] = 0;
        }
        
    }
}


void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(debug_led1);
}

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
    printf("system starting tasks...\n");

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