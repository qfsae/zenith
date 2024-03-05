/**
 * @file interface_sysError.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Error reporting interface
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 * Interface for system error reporting to 7 segment displays.
 * Console logging can be enable with the `PRINTF_REPORT_ERR` macro.
 * 
 * *Note: Seven Segment Displays are only available on VCU Hardware >= V2*
 * 
 */

#pragma once
#include "interfaces/interface_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tasks/taskHandlers.h"
#include "stream_buffer.h"
#include "errors.h"

//  Enable console logging of errors
#define PRINTF_REPORT_ERR

/**
 * @brief OS Error Setup Handler
 * 
 * This function should be called to init the error arrays and handler interfaces
 * This function should not be called more than once
 * 
 * Called in main
 * 
 */
extern void os_sysError_setup(void);

/**
 * @brief Report a system error. Passing `SYS_OK` will do nothing
 * 
 * @param error <enum SYS_ERROR> The error to report
 */
extern void sysError_report(enum SYS_ERROR error);

/**
 * @brief Report a system error from a task (Suspends the task)
 * 
 * @param error <enum SYS_ERROR> The error to report
 * @param task <TaskHandle_t> The task that reported the error
 */
extern void sysError_reportTsk(enum SYS_ERROR error, TaskHandle_t *task);

/**
 * @brief Clear a system error
 * 
 * @param error <enum SYS_ERROR> The error to clear
 */
extern void sysError_clear(enum SYS_ERROR error);

/**
 * @brief Check if an error is currently active
 * 
 * @param error <enum SYS_ERROR> The error to check
 * @return true if the error is active
 * @return false if the error is clean
 */
extern bool sysError_check(enum SYS_ERROR error);

/**
 * @brief Check if an error has been active since last reset
 * 
 * @param error <enum SYS_ERROR> The error to check
 * @return true if the error was active at any time
 * @return false if the error has never been active
 */
extern bool sysError_checkRuntime(enum SYS_ERROR error);


