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


static uint16_t led1 = PIN('B', 0);
static uint16_t led2 = PIN('B', 1);

// test task for blinking an led
void testTask(void *param){
    (void)(param); // Cast unused variable to void
    for(;;){
        gpio_toggle_pin(led2);
        vTaskDelay(1000);
    }
}

void TIM6_DAC_IRQHandler(){
    // Place at beginning of IQR - Otherwise it causes the NVIC to rerun the IQR
    TIM6->SR = TIM_SR_CC1IF;
    gpio_toggle_pin(led1);
}

int main(void){

    // set up gpio
    gpio_set_mode(led1, GPIO_MODE_OUTPUT);
    gpio_write(led1, true);
    gpio_set_mode(led2, GPIO_MODE_OUTPUT);
    gpio_write(led2, false);

    // Enable Timer 6 (Basic Timer) 1Hz (APB2/45000, count to 2000)
    TIM_basic_Init(TIM6, 45000U, 2000U);
    NVIC_SetPriority(TIM6_DAC_IRQn, 0x03);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // set up a basic test task
    TaskHandle_t gTestTask = NULL;

    uint32_t status = xTaskCreate(
        testTask,
        "TestTsk",
        1024,
        NULL,
        tskIDLE_PRIORITY,
        &gTestTask
    );

    vTaskStartScheduler();

    // System Main loop (Should never run -> Scheduler runs infinitely)
    for(;;) {
        // Write out Error LED
        gpio_write(led2, false);
    }
    return 0;
}