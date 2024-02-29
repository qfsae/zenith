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
#include "FreeRTOS.h"


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

/**
 * @brief Get most current copy of a CAN message based on its ID
 * 
 * @param CAN The Physical CAN Bus the message was received on 
 * @param id The CAN ID of the message
 * @return can_msg_t 
 */
extern can_msg_t can_fetch(CAN_TypeDef *CAN, uint32_t id);

/**
 * @brief Check if a CAN message has gone stale based on its receive time
 * 
 * @param CAN The Physical CAN Bus the message was received on
 * @param id The CAN ID of the message
 * @param maxTicks The maximum time before a message is considered stale.
 * @return true if the message is still good
 * @return false if the message is out of date
 */
extern bool can_check_timestamp(CAN_TypeDef *CAN, uint32_t id, uint32_t maxTicks);

/**
 * @brief Send a message on the CAN Bus
 * 
 * @param CAN The Physical CAN bus to use
 * @param tx_msg The CAN Message to send
 * @param timeout The Timeout to wait to claim a CAN mailbox
 * @returns SYS_OK or the associated error
 */
extern uint8_t can_send_msg(CAN_TypeDef *CAN, can_msg_t *tx_msg, TickType_t timeout);


