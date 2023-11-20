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

TaskHandle_t tskh_Test1 = NULL;
TaskHandle_t tskh_BlinkLED = NULL;
TaskHandle_t tskh_USART2_Handler = NULL;

void tsk_Test1(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        // Toggle LED Pin
        // gpio_toggle_pin(debug_led2);
        printf("%d\n", xTaskGetTickCount());
        vTaskDelay(1000);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    for(;;){
        // if(hal_uart_read_ready(debug.interface)){
        //     uint8_t receivedData;

        //     // Read data from the USART (replace with your actual read operation)
        //     receivedData = hal_uart_read_byte(debug.interface);

        //     // Send the received data to the task
        //     xQueueSend(debug.rxQue, (void*) &receivedData, portMAX_DELAY);
        // }
        vTaskSuspend(NULL);
    }
}

void tsk_USART2_Handler(void *param){
    (void)param;
    for(;;){
        uint8_t buf = 0;
        uint32_t pendingBit;
        xTaskNotifyWaitIndexed( 0,                  /* Wait for 0th Notificaition */
                                0x00,               /* Don't clear any bits on entry. */
                                ULONG_MAX,          /* Clear all bits on exit. */
                                &pendingBit, /* Receives the notification value. */
                                portMAX_DELAY );    /* Block indefinitely. */
        printf("%c", pendingBit);
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
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03); // Enable Timer IRQ (lowest priority)
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
    
    
    xTaskCreate(
        tsk_USART2_Handler,
        "u2",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &tskh_USART2_Handler
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