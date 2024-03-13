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
#include "core.h"
#include "tasks.h"

#define taskResume(tsk) vTaskResume(xTaskHandles[tsk])
#define taskSuspend(tsk) vTaskSuspend(xTaskHandles[tsk])

/* BEGIN System State Control Code */

enum SYS_STATE core_state = SYS_STATE_INIT;

enum SYS_STATE core_get_state(void){
    return core_state;
}

/* END System State Control Code */


void vTask_Core(void *param){
    (void) param;
    uart_write(&Serial2, "Started Core Task\n", 18, 10);
    // Initialize all tasks to a disabled state
    task_init();
    task_printDebug(&Serial2);
    printf("Core Task Started\n");
    core_state = SYS_STATE_IDLE;

    for(;;){
        // Loop through system states, checking for errors after each state releases control
        // More information in header file
        vState_Idle(&core_state);
        vState_Error(&core_state);

        vState_Start(&core_state);
        vState_Error(&core_state);

        vState_RTD(&core_state);
        vState_Error(&core_state);
    } // END Core Task for loop
} // END Core Task

/* BEGIN Error Handling Code */

uint32_t errors[3];
#define get_e(e) (bool)((errors[e/3] >> (e % 32)) & 0x01)
#define set_e(e) ((errors[e/3] |= 0x1 << (e % 32)))

void core_throw(enum SYS_ERROR e, TaskHandle_t *pTask){
    if(e == SYS_OK) return;
    set_e(e);
    // Force an idle system state
    core_state = SYS_STATE_IDLE;
    /* If the error originated from the core task,
     * return the system to an idle state.
     * If the error was reported externally,
     * suspend the offending task
     */
    if(pTask == &xCoreTaskHandle){
        return;
    }
    vTaskSuspend(*pTask);
}

void core_throw_iqr(enum SYS_ERROR e){
    if(e == SYS_OK) return;
    set_e(e);
}

bool core_check_error(enum SYS_ERROR e){
    return get_e(e);
}

bool core_check_fault(void){
    return (errors[0] == 0 && errors[1] == 0 && errors[2] == 0);
}

/* END Error Handling Code */
