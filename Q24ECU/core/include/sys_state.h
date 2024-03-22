/**
 * @file core.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Main System Runtime Header
 * @version 0.1
 * @date 2024-03-04
 * 
 * This file serves as the header file for the main system runtime
 * The Core is responsible for system startup, excluding the bootloader, and hardware interfaces
 * that could prevent the system from starting up properly.
 * 
 * The Core's main function is to initialize and maintain the acceleration throttle control (ATC)
 * tasks including the ATC task itself and the Software Differential Control (SDC) task.
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "errors.h"
#include <stdbool.h>

/* BEGIN System State Control Code */

// System State Tracker
enum SYS_STATE {
    SYS_STATE_INIT,
    SYS_STATE_IDLE,
    SYS_STATE_TEST,
    SYS_STATE_TS_ACTIVE,
    SYS_STATE_RTD,
    SYS_STATE_ERR
};

extern enum SYS_STATE controller_get_state(void);

/* END System State Control Code */

extern TaskHandle_t xControllerTaskHandle;
extern void vTask_Controller(void *param);

/* BEGIN System State Functions */

/*  There is no initialization state, Initialization should happen directly within the core
    function. Idle tasks should be resumed within the vState_Idle function

    States run in the order specified below, and within the core task. Each state is responsible
    for managing the system during its runtime. Once a state has completed, it must exit to the core task.
    Upon returning to the core task, the system state will be checked against the error state before the
    next state starts. States are looping, ie the Ready To Drive state returns to the idle state.
*/

/**
 * @brief System IDLE State
 * 
 * @param state Pointer to the current system state
 * @returns When the Tractive System Enable Switch is ON
 */
extern void vState_Idle(enum SYS_STATE *state);

/**
 * @brief Tractive System Start State
 * 
 * @param state Pointer to the current system state
 * 
 * @returns When Pre-Charge has completed and the driver has completed the steps to enable driving
 */
extern void vState_Start(enum SYS_STATE *state);

/**
 * @brief Ready To Drive State (Runs while the car responds to APPS input)
 * 
 * @param state Pointer to the current system state
 * @returns When the driver shuts down the car, or when an error is encountered
 */
extern void vState_RTD(enum SYS_STATE *state);

/**
 * @brief System Error Checking State
 * This state function checks to see if an error is present,
 * and attempts to solve it.
 * @param state Pointer to the current system state
 * @returns if the error is resolvable
 */
extern void vState_Error(enum SYS_STATE *state);

/* END System State Functions */

/* BEGIN System Error Handling */

/**
 * @brief Throw an error to the system state controller
 * 
 * @param e The SYS_ERROR to throw
 */
extern void controller_throw(enum SYS_ERROR e);

/**
 * @brief Check if an error is present in the system state controller
 * 
 * @param e The Error to check
 * @return true if the error is present
 * @return false if the error has not been reported
 */
extern bool controller_check_error(enum SYS_ERROR e);

/**
 * @brief Check if any faults have been reported to the system state controller
 * 
 * @return true if an unresolved error has been reported
 */
extern bool controller_check_fault(void);

/* END System Error Handling */



