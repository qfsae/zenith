/**
 * @file shutdown.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-02-18
 * 
 * @copyright Copyright (c) 2024
 * 
 * Shutdown handler for the VCU
 * 
 * This file contains the shutdown code for the VCU to disable the tractive system
 * Code should be called through the error handler with `TASK_SHUTDOWN_CRITICAL`
 * passed as the error argument
 * 
 */
#include "main.h"
#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Shutdown the VCU
 * 
 * This function should be called when a critical error is detected
 * This function will disable all power output and shutdown the scheduler
 * 
 * The system should only reach this function when a non-recoverable,
 * internal error is detected. This function provides NO GUARANTEES that
 * the system will be in a safe state after execution, or that the HV system
 * will be shut down.
 * 
 * @note This function will not return
 * 
 */
void system_critical_shutdown(void){
    // Disable all interrupts
    printf("INVOKING CRITICAL SHUTDOWN\n");
    __disable_irq();
    printf("IRQs Disabled\n");

    // Disable all power outputs
    printf("Disabling Power Outputs\n");

    // Shutdown all tasks
    vTaskSuspendAll();
    vTaskEndScheduler();

    // Scheduler will now enter into an indefinite loop


    // Should never reach here
    for(;;){
        __asm__("NOP");
    }

}