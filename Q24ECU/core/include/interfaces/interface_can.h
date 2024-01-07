/**
 * @file interface_can.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief FreeRTOS CAN bus Interface for QFSAE VCU
 * @version 0.1
 * @date 2023-12-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "hal/hal_can.h"


/**
 * @brief OS CAN Setup Handler
 * 
 * This function should be called to init the CAN bus and handler interfaces
 * This function should not be called more than once
 * ALL CAN bus initialization should happen within this function
 * 
 * Called in main
 * 
 */
extern void os_can_setup(void);

extern void CAN1_RX0_IRQHandler(void);

extern can_msg_t can_fetch(CAN_TypeDef *CAN, uint32_t id);

extern bool can_check_timestamp(CAN_TypeDef *CAN, uint32_t id, uint32_t maxTicks);


extern void can_send_msg(CAN_TypeDef *CAN, can_msg_t *tx_msg);


