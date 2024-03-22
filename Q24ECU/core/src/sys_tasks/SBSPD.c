/**
 * @file SBSPD.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys_tasks.h"


void vTask_SBSPD(void *param){
    (void)param;
    for(;;){
        vTaskDelay(200);
    }
}