/**
 * @file testTasks.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Test Task file - Collection of test tasks
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "tasks/taskHandlers.h"
#include "main.h"
#include "task.h"
#include "interfaces/interface_uart.h"
#include "interfaces/interface_can.h"
#include "interfaces/interface_sysError.h"
#include "hal/hal_adc.h"
#include "hal/legacy_hal_adc.h"
#include <stdio.h>
#include "string.h"
#include "interfaces/interface_adc.h"

uint16_t adcread = 99;
uint16_t adcread2 = 99;
bool adc1 = false;

long unsigned int counter = 0;

void vTask_Test1(void *param){
    (void)(param); // Cast unused variable to void

    for (;;)
    {
        char buf[40];
        snprintf(buf, 40, "%0.4f %0.4f\n", adc_fetch(ADC_CHANNEL_5V1), adc_fetch(ADC_CHANNEL_5V2));
        uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 10U);
        vTaskDelay(100);
    }
}

void vTask_BlinkLED(void *param){
    (void)param;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for(;;){
        lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, 100);
    }
}

