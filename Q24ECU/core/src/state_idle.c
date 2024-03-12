/**
 * @file state_idle.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief System Idle State Code
 * @version 0.1
 * @date 2024-03-11
 * 
 * This function runs when the system is idleing.
 * It must monitor for the TS Enable switch. Once the switch has
 * been pressed, this task must switch the state to TS_Active
 *
 * @copyright Copyright (c) 2024
 * 
 */

#include "core.h"
#include "tasks.h"

void vState_Idle(enum SYS_STATE *state){
    // Check to see if the system state matches the function state
    if(*state != SYS_STATE_IDLE) return;
    // Enable the error task
    if(eTaskGetState(xTaskHandles[eTask_ErrorScreen]) == eSuspended)
        vTaskResume(xTaskHandles[eTask_ErrorScreen]);
    // Enable the SBSPD task
    if(eTaskGetState(xTaskHandles[eTask_SBSPD]) == eSuspended)
        vTaskResume(xTaskHandles[eTask_SBSPD]);
    // disable inverters
    // disable shutdown circuit
    for(;;){
        if(false){
            *state = SYS_STATE_TS_ACTIVE;
            return;
        }
        if(core_check_fault()){
            // fault handling
            *state = SYS_STATE_ERR;
            return;
        }
        vTaskDelay(20);
    }
}

