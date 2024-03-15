/**
 * @file errorScreen.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys_tasks.h"
#include "task.h"
#include "sys_state.h"

void vTask_ErrorScreen(void *param){
    (void)param;
    for(;;){
        // for(int i = SYS_OK; i <= SYS_ERROR_MAX; i++){
        //     if(core_check_error(i)){
        //         // Display Error on 7 segment
        //         vTaskDelay(1000);
        //     }
        // }
        vTaskDelay(100);
    }
}


