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

enum SYS_STATE {
    SYS_STATE_INIT,
    SYS_STATE_IDLE,
    SYS_STATE_TEST,
    SYS_STATE_RUN,
    SYS_STATE_ERR
};

extern enum SYS_STATE core_get_state();

extern void core_throw(enum SYS_ERROR e, TaskHandle_t *pTask);

extern void core_throw_iqr(enum SYS_ERROR e);

extern TaskHandle_t xCoreTaskHandle;

extern void vTask_Core(void *param);


