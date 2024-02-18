/**
 * @file interface_sysError.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Error reporting interface
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "interfaces/interface_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "taskHandlers.h"
#include "stream_buffer.h"
#include "errors.h"

#define PRINTF_REPORT_ERR

#define varStr(x) #x

extern void os_sysError_setup(void);

extern void sysError_report(enum SYS_ERROR error);

extern void sysError_reportTsk(enum SYS_ERROR error, TaskHandle_t *task);

extern void sysError_clear(enum SYS_ERROR error);

extern bool sysError_check(enum SYS_ERROR error);



