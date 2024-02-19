/**
 * @file interface_sysError.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
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

#include "interfaces/interface_sysError.h"
#include "taskhandlers.h"
#include "inttypes.h"
#include "main.h"

// Internal

// Macros for bit manipulation
#define byteIndex(error) (uint8_t)(error / 8)
#define bitIndex(error)  (uint8_t)(error % 8)

// Currently active system errors
uint8_t system_errors[byteIndex(SYS_ERROR_MAX) + 1];
// Active errors as of last check
uint8_t system_errors_prev[byteIndex(SYS_ERROR_MAX) + 1];
// Errors reported (Only resets on power cycle)
uint8_t system_errors_persist[byteIndex(SYS_ERROR_MAX) + 1];

/**
 * @brief Check if an error has already been logged
 * 
 * @param error <enum SYS_ERROR> The error to check
 * @return true if the error exists within the system_errors_prev array
 * @return false if the error is new
 */
bool sysError_checkExisting(enum SYS_ERROR error){
    return (system_errors_prev[byteIndex(error)] >> bitIndex(error)) & 0x01U;
}

/**
 * @brief Display the error on the 7 segment displays
 * 
 * @param error <enum SYS_ERROR> The error to display
 */
void displayError(enum SYS_ERROR error){
    (void)error;
    // TODO: Display the error on the 7 segment displays

    // Used for debugging
    // printf("Displaying Error: %d\n", error);
}

// Public

void os_sysError_setup(void){
    // Clear all errors
    for(int i = 0; i < byteIndex(SYS_ERROR_MAX); i++){
        system_errors[i] = 0;
        system_errors_prev[i] = 0;
        system_errors_persist[i] = 0;
    }
    displayError(SYS_OK);
}

void sysError_report(enum SYS_ERROR error){
    // Check if no error is present
    if(error == SYS_OK) return;
    // Check if the error is system critical
    if(error == TASK_SHUTDOWN_CRITICAL) {
        taskENTER_CRITICAL();
        displayError(TASK_SHUTDOWN_CRITICAL);
        // Invoke the system critical shutdown function
        system_critical_shutdown();
        // Should never reach here
    }
    system_errors[byteIndex(error)] |= (uint8_t)(1U << bitIndex(error));
    vTaskResume(xTaskHandles[eTask_SysError]);
}

void sysError_reportTsk(enum SYS_ERROR error, TaskHandle_t *task){
    sysError_report(error);
    #ifdef PRINTF_REPORT_ERR
    printf("Suspending due to error %d\n", error);
    #endif
    vTaskResume(xTaskHandles[eTask_SysError]);
    vTaskSuspend(*task);
}

void sysError_clear(enum SYS_ERROR error){
    vTaskResume(xTaskHandles[eTask_SysError]);
    system_errors[byteIndex(error)] &= (uint8_t)(~(1U << bitIndex(error)));
    
}

bool sysError_check(enum SYS_ERROR error){
    return (system_errors[byteIndex(error)] >> bitIndex(error)) & 0x01U;
}

bool sysError_checkRuntime(enum SYS_ERROR error){
    return (system_errors_persist[byteIndex(error)] >> bitIndex(error)) & 0x01U;
}

// Task for handling system errors
void vTask_SysError(void *param){
    (void)param;
    // Delay for 1 tick to allow the system to start
    vTaskDelay(1);

    for(;;){
        // Initialize variable to check if there are any errors present
        bool error = false;
        // Check all the errors
        for (int i = 0; i <= SYS_ERROR_MAX; i++) {
            // Check if a new error has spawned
            if(sysError_check((enum SYS_ERROR)i) == true && sysError_checkExisting((enum SYS_ERROR)i) == false){
                #ifdef PRINTF_REPORT_ERR
                printf("%d\n", i);
                #endif
                system_errors_prev[byteIndex(i)] |= (uint8_t)(1U << bitIndex(i));
                system_errors_persist[byteIndex(i)] |= (uint8_t)(1U << bitIndex(i));
                error = true;
            }
            // Check if a preexisting error has been cleared
            else if(sysError_check((enum SYS_ERROR)i) == false && sysError_checkExisting((enum SYS_ERROR)i) == true){
                #ifdef PRINTF_REPORT_ERR
                printf("Cleared %d\n", i);
                #endif
                system_errors_prev[byteIndex(i)] &= (uint8_t)(~(1U << bitIndex(i)));
            }

            // If an error is present, set the error flag
            if(sysError_check((enum SYS_ERROR)i) == true){
                error = true;
                // Display the error on the 7 segment displays
                displayError((enum SYS_ERROR)i);
                // Persist for 1 second, then loop through the rest of the errors
                vTaskDelay(1000);
            }
        }

        if(error == false){
            // Clear the 7 segment displays
            displayError(SYS_OK);
            #ifdef PRINTF_REPORT_ERR
            // Report SYS_OK to the terminal
            printf("SYS_OK\n");
            #endif
            // Suspend the task indefinitely (Will resume when an error is reported via `sysError_report` or `sysError_reportTsk`)
            vTaskSuspend(NULL);
        }
    }
}