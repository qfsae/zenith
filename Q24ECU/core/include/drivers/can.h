/**
 * @file can.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief FreeRTOS CAN bus Interface for QFSAE VCU
 * @version 1.1
 * @date 2024-03-04
 * 
 * Version 1.0
 * - Created CAN Interface code
 * Version 1.1
 * - Refactored as a driver
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "stm32f4xx.h"
#include "stdbool.h"
#include "hal/hal_can.h"
#include "FreeRTOS.h"


/**
 * @brief CAN Initialization Handler
 * 
 * This function will initialize the CAN bus and handler interfaces
 * This function should not be called more than once
 * 
 * @note This Driver initializes a FreeRTOS Task in order to function
 */
extern void can_init(void);


/**
 * @brief Get most current copy of a CAN message based on its ID
 * 
 * @param CAN The Physical CAN Bus the message was received on 
 * @param id The CAN ID of the message
 * @return can_msg_t 
 */
extern can_msg_t can_read(CAN_TypeDef *CAN, uint32_t id);


/**
 * @brief Send a message on the CAN Bus
 * 
 * @param CAN The Physical CAN bus to use
 * @param tx_msg The CAN Message to send
 * @param timeout The Timeout to wait to claim a CAN mailbox
 * @returns SYS_OK or the associated error
 */
extern uint8_t can_write(CAN_TypeDef *CAN, can_msg_t *tx_msg, TickType_t timeout);

/* All IRQ Handlers must be globally accessible for linking purposes */

// CAN1 Receive IRQ Handler
extern void CAN1_RX0_IRQHandler(void);