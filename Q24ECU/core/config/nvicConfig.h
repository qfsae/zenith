/**
 * @file nvicConfig.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 * This file exists because of some odd errors surrounding FreeRTOS and the NVIC.
 * 
 * In order to avoid deadlocks within an interrupt: Do NOT exceed these parameters.
 * EX:
 *      NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MAX);
 *      NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MAX+5);
 *      NVIC_SetPriority(USART2_IRQn, NVIC_Priority_MIN-10);
 * 
 */

#pragma once

// Highest Priority that can be given to an NVIC Interrupt
#define NVIC_Priority_MAX 21
// Lowest Priority that can be given to an NVIC Interrupt
#define NVIC_Priority_MIN 79

