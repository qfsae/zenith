/**
 * @file interfaces.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Global defines for interface tasks
 * @version 0.1
 * @date 2024-03-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

// Located in interface_uart.c
extern void vTask_USART2_Handler(void *param);

// Located in interface_can.c
extern void vTask_CAN_RXBufferHandler(void *param);

// Located in interface_sysError.c
extern void vTask_SysError(void *param);

// Located in interface_adc.c
extern void vTask_ADCMonitor(void *param);


