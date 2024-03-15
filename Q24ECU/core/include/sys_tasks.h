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
    // CAN Bus Reporting Task
    eTask_CANSend,
    // Keep this last
    eTask_TaskCount // Number of tasks running on the system (`taskHandlers.h`)
};

/* Global define for all system task handles */
extern TaskHandle_t xTaskHandles[eTask_TaskCount];

/**
 * @brief Suspend a task if it is currently running
 * 
 * @param task The Task to suspend
 */
static inline void task_Suspend(enum Tasks task){
    if(eTaskGetState(xTaskHandles[task]) != eSuspended)
        vTaskSuspend(xTaskHandles[task]);
}

/**
 * @brief Resume a previously suspended task
 * 
 * @param task 
 */
static inline void task_Resume(enum Tasks task){
    if(eTaskGetState(xTaskHandles[task]) == eSuspended)
        vTaskResume(xTaskHandles[task]);
}

/**
 * @brief Print out debug information about all tasks on the system
 * 
 * @param Serial The Serial port to write the information to
 */
static inline void task_printDebug(UART_Handle_t *Serial){
    char buf[100];
    snprintf(buf, 100, "Task:\t   Identifier\t    Priority\t\tState\n");
    uart_write_str(Serial, buf, 10);
    for (int i = 0; i < eTask_TaskCount; i++)
    {
        snprintf(buf, 100, "%d: %16s\t\t%lu\t\t", i, pcTaskGetName(xTaskHandles[i]), uxTaskPriorityGet(xTaskHandles[i]));
    uart_write_str(Serial, buf, 10);
        switch(eTaskGetState(xTaskHandles[i])) {
            case eRunning:
                uart_write(Serial, "RUNNING\n", 9, 10);
                break;
            case eReady:
                uart_write(Serial, "READY\n", 6, 10);
                break;
            case eBlocked:
                uart_write(Serial, "BLOCKED\n", 8, 10);
                break;
            case eSuspended:    
                uart_write(Serial, "SUSPENDED\n", 10, 10);
                break;
            case eDeleted:   
                uart_write(Serial, "DELETED\n", 8, 10);
                break;
            case eInvalid:   
                uart_write(Serial, "INVALID\n", 8, 10);
                break;
            default:
                uart_write(Serial, "UNKNOWN\n", 8, 10);
                break;
        }
    }
}


/**
 * @brief Used to initialize all system tasks 
 * Tasks are initialized to a halted state
 * 
 */
extern void tasker_init(void);

/**
 * @brief Helper function to print out all data pertaining to the systems tasks
 * 
 */
extern void task_printDebug(UART_Handle_t *Serial);

/*  BEGIN Global Definitions of all System Tasks */

extern void vTask_SBSPD(void *param);

extern void vTask_ErrorScreen(void *param);

extern void vTask_Test1(void *param);

extern void vTask_CAN_send(void *param);

/*  END Global Definitions of all System Tasks */



