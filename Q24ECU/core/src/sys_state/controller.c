/**
 * @file core.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Main System Runtime Header
 * @version 0.1
 * @date 2024-03-04
 * 
 * This file serves as the header file for the main system runtime
 * The Core program is responsible for system startup, excluding the bootloader, and hardware interfaces that could prevent the system from starting up properly.
 * 
 * The Core's main function is to initialize and maintain the acceleration throttle control (ATC)
 * tasks including the ADT task itself and the Software Differential Control (SDC) task.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include "sys_state.h"
#include "sys_tasks.h"

#define taskResume(tsk) vTaskResume(xTaskHandles[tsk])
#define taskSuspend(tsk) vTaskSuspend(xTaskHandles[tsk])

/* BEGIN System State Control Code */

enum SYS_STATE core_state = SYS_STATE_INIT;

enum SYS_STATE core_get_state(void){
    return core_state;
}

/* END System State Control Code */


void vTask_Controller(void *param){
    (void) param;
    uart_write(&Serial2, "Started Core Task\n", 18, 10);
    // Initialize all tasks to a disabled state
    tasker_init();
    task_printDebug(&Serial2);
    uart_write_str(&Serial2, "Core Task Started\n", 10);
    // Initialize the system into the idle state
    core_state = SYS_STATE_IDLE;

    for(;;){
        // Loop through system states, checking for errors after each state releases control
        // More information in header file
        switch (core_state) {
        case SYS_STATE_IDLE:
            vState_Idle(&core_state);
            break;
        // Both test cases and active require running the same start sequences
        case SYS_STATE_TEST:
        case SYS_STATE_TS_ACTIVE:
            vState_Start(&core_state);
            break;
        case SYS_STATE_RTD:
            vState_RTD(&core_state);
            break;
        default:
            vState_Error(&core_state);
            break;
        }
        // Check to see if the system is in an error state
        vState_Error(&core_state);
    } // END Core Task for loop
} // END Core Task

