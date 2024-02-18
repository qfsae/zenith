/**
 * @file interface_sysError.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "interfaces/interface_sysError.h"
#include "taskhandlers.h"
#include "inttypes.h"

#define byteIndex(error) (uint8_t)(error / 8)
#define bitIndex(error)  (uint8_t)(error % 8)

uint8_t system_errors[byteIndex(SYS_ERROR_MAX) + 1];
uint8_t system_errors_prev[byteIndex(SYS_ERROR_MAX) + 1];

void os_sysError_setup(void){
    // Clear all errors
    for(int i = 0; i < byteIndex(SYS_ERROR_MAX); i++){
        system_errors[i] = 0;
    }
}

void sysError_report(enum SYS_ERROR error){
    if(error == SYS_OK) return;
    printf("ERROR: %d\n", error);
    system_errors[byteIndex(error)] |= (uint8_t)(1U << bitIndex(error));
}

void sysError_reportTsk(enum SYS_ERROR error, TaskHandle_t *task){
    system_errors[byteIndex(error)] |= (uint8_t)(1U << bitIndex(error));
    printf("Suspending %s due to error: %d\n", pcTaskGetName(*task), error);
    vTaskSuspend(*task);
}

void sysError_clear(enum SYS_ERROR error){
    system_errors[byteIndex(error)] &= (uint8_t)(~(1U << bitIndex(error)));
}

bool sysError_check(enum SYS_ERROR error){
    return (system_errors[byteIndex(error)] >> bitIndex(error)) & 0x01U;
}

bool sysError_checkExisting(enum SYS_ERROR error){
    return (system_errors_prev[byteIndex(error)] >> bitIndex(error)) & 0x01U;
}

void vTask_SysError(void *param){
    (void)param;
    vTaskDelay(1);
    for(;;){
        bool error = false;
        printf("Checking for errors\n");
        for (int i = 0; i <= SYS_ERROR_MAX; i++)
        {
            char buf[40];
            snprintf(buf, 40, "%d: %d %d\n", i, sysError_check((enum SYS_ERROR)i), sysError_checkExisting((enum SYS_ERROR)i));
            uart_send_buf_blocking(&port_uart4, buf, strlen(buf), 1000);
            if(sysError_check((enum SYS_ERROR)i) == true && sysError_checkExisting((enum SYS_ERROR)i) == false){
                printf("%d\n", i);
                system_errors_prev[byteIndex(i)] |= (uint8_t)(1U << bitIndex(i));
                error = true;
            }
            else if(sysError_check((enum SYS_ERROR)i) == false && sysError_checkExisting((enum SYS_ERROR)i) == true){
                printf("Cleared %d\n", i);
                system_errors_prev[byteIndex(i)] &= (uint8_t)(~(1U << bitIndex(i)));
            }
            if(sysError_check((enum SYS_ERROR)i) == true){
                error = true;
                vTaskDelay(1000);
            }
        }
        
        if(error == false){
            printf("SYS_OK\n");
            vTaskDelay(5000);
        }
    }
}