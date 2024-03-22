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

#include "sys_state.h"
#include "sys_tasks.h"

void vState_Idle(enum SYS_STATE *state){
    // Check to see if the system state matches the function state
    if(*state != SYS_STATE_IDLE) return;
    // Enable the error task
    // if(eTaskGetState(xTaskHandles[eTask_ErrorScreen]) == eSuspended)
    //     vTaskResume(xTaskHandles[eTask_ErrorScreen]);
    // // Enable the SBSPD task
    // if(eTaskGetState(xTaskHandles[eTask_SBSPD]) == eSuspended)
    //     vTaskResume(xTaskHandles[eTask_SBSPD]);
    task_Resume(eTask_TEST1);
    task_Resume(eTask_CANSend);
    printf("System is now in the Idle State\n");
    task_printDebug(&Serial2);
    // disable inverters
    // disable shutdown circuit
    for(;;){
        if(controller_check_fault() == true){
            *state = SYS_STATE_ERR;
            return;
        }
        vTaskDelay(200);
    }
}

