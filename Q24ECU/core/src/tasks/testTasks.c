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
#include "interrupts.h"
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
    // printf("Running Task 1\n");
    // can_msg_t incoming;
    // printf("setup");
    // for(;;){
    //     incoming = can_fetch(CAN1, 200);
    //     printf(">RPM:%d\n", incoming.data[0]);
    //     incoming.len = 1;
    //     incoming.id = 99;
    //     can_send_msg(CAN1, &incoming, portMAX_DELAY);
    //     vTaskDelay(200);
    // }
    // hal_adc_init(ADC1, ADC_RESOLUTION_12_BIT);

    // SET_BIT(ADC123_COMMON->CCR, 3 << 16);
    // gpio_set_mode(PIN('A', 4), GPIO_MODE_ANALOG);
    // gpio_set_mode(PIN('A', 5), GPIO_MODE_ANALOG);

    // hal_adc_configChannel(ADC1, 4, ADC_CYCLES_3, ADC_SQ1);
    // hal_adc_set_sequence_len(ADC1, 1);
    // hal_adc_enable(ADC1);
    // hal_adc_startConversions(ADC1);
    // hal_adc_init(ADC1, ADC_RESOLUTION_12_BIT);
    // SET_BIT(ADC1->CR1, ADC_CR1_EOCIE);
    // NVIC_EnableIRQ(ADC_IRQn);
    // NVIC_SetPriority(ADC_IRQn, NVIC_Priority_MIN);
    // hal_adc_configChannel(ADC1, 4, ADC_CYCLES_112, ADC_SQ1);
    // hal_adc_configChannel(ADC1, 5, ADC_CYCLES_112, ADC_SQ2);
    // // legacy_hal_adc_configChannel(ADC1, 4, 1);
    // hal_adc_set_sequence_len(ADC1, 2);
    // hal_adc_startConversions(ADC1);
    // CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L);
    // SET_BIT(ADC1->SQR1, (uint32_t)(1UL << ADC_SQR1_L_Pos));:w
    


    // legacy_hal_adc_init(ADC2);
    
    for(;;) {
        // if(READ_BIT(ADC1->CR2, ADC_CR2_SWSTART) == 0){
            
        // }
        // if(READ_BIT(ADC1->SR, ADC_SR_EOC)){
            char buf[40];
            // if(READ_BIT(ADC1->SR, ADC_SR_OVR)){
            //     printf("Overflow\n");
            // }
            // hal_adc_startConversions(ADC1);
            // SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
            // while(!(ADC1->SR & ADC_SR_EOC));
            snprintf(buf, 40, "%0.4f %0.4f\n", adc_fetch(ADC_CHANNEL_5V1), adc_fetch(ADC_CHANNEL_5V2));
            uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 10U);
        //     CLEAR_BIT(ADC1->SR, ADC_SR_EOC);
            // SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
        // }
        vTaskDelay(100);
    }
}


void vTask_BlinkLED(void *param){
    (void)param;
    // TickType_t lastWakeTime = xTaskGetTickCount();
    // for(;;){
    //     // This task is currently not used so perma suspend it
    //     lastWakeTime = xTaskGetTickCount();
    //     counter++;
    //     can_msg_t msg;
    //     msg.id = 98;
    //     msg.len = 1;
    //     msg.data[0] = 22;
    //     can_send_msg(CAN1, &msg, portMAX_DELAY);
    //     vTaskDelayUntil(&lastWakeTime, 100);
    // }
    vTaskSuspend(NULL);
}

