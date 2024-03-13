/**
 * @file tasks.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Central collection of all system tasks + initialization functions
 * @version 1.0
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "drivers/uart.h"

enum Tasks{
    // Software BSPD (Brake System Plausibility Device)
    eTask_SBSPD,
    // 7 Segment Error Screen Control Task
    eTask_ErrorScreen,
    // Test Task
    eTask_TEST1,
    // Keep this last
    eTask_TaskCount // Number of tasks running on the system (`taskHandlers.h`)
};

/* Global define for all system task handles */
extern TaskHandle_t xTaskHandles[eTask_TaskCount];

static inline void task_Suspend(enum Tasks task){
    if(eTaskGetState(xTaskHandles[task]) != eSuspended)
        vTaskSuspend(xTaskHandles[task]);
}

static inline void task_Resume(enum Tasks task){
    if(eTaskGetState(xTaskHandles[task]) == eSuspended)
        vTaskResume(xTaskHandles[task]);
}

/**
 * @brief Used to initialize all system tasks 
 * Tasks are initialized to a halted state
 * 
 */
extern void task_init(void);

/**
 * @brief Helper function to print out all data pertaining to the systems tasks
 * 
 */
extern void task_printDebug(UART_Handle_t *Serial);

/*  BEGIN Global Definitions of all System Tasks */

extern void vTask_SBSPD(void *param);

extern void vTask_ErrorScreen(void *param);

extern void vTask_Test1(void *param);

/*  END Global Definitions of all System Tasks */



