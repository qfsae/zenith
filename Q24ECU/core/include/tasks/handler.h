/**
 * @file handler.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Global definitions of all task handlers defined
 * @version 0.1
 * @date 2023-11-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

enum Tasks{
    // Interface Tasks
    eTask_USART2_Handler,
    eTask_CAN_rxBufferHandler,
    eTask_SysError,
    eTask_ADCMonitor,
    // System Tasks
    eTask_Test1,
    eTask_BlinkLED,
    eTask_CAN_send,
    eTask_CAN_receive,
    // Keep this last
    eTask_TaskCount // Number of tasks running on the system (`taskHandlers.h`)
};

/* Global define for all system task handles */
extern TaskHandle_t xTaskHandles[eTask_TaskCount];


/**
 * @brief Used to initialize all tasks pertaining to the system interfaces
 * 
 */
extern void tasksHandler_initInterfaces(void);

/**
 * @brief Used to initialize all system tasks (not interface tasks)
 * Tasks are initialized to a halted state
 * 
 */
extern void tasksHandler_initTasks(void);

/**
 * @brief Helper function to print out all data pertaining to the systems tasks
 * 
 */
extern void tasksHandler_printDebug(void);
