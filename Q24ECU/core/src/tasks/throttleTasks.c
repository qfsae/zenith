/**
 * @file adcTasks.c
 * @author Matt Dobaj
 * @brief Collection of tasks that relate to Throttle Control
 * @version 0.1
 * @date 2023-12-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "hal/hal_adc.h"

#include "taskHandlers.h"
#include "main.h"

#define throttlePedalCh 1

void tsk_UpdateThrottle(void *param) {
    (void)param;
    for(;;){
        // Print out the throttle's position twice per second.
        uint16_t throttlePos = adc_poll(ADC1, throttlePedalCh);
        printf("Throttle: %d\n", throttlePos);
        vTaskDelay(500);
    }
}
