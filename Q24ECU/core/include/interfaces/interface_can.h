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
//#include "FreeRTOS.h"
//#include "task.h"
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

extern can_msg_t can_fetch(uint32_t id);


/**
 * @brief Fetch a CAN bus data byte via a given ID
 * 
 * @param id The ID associated with the data
 * @param byte The byte index
 * @return uint8_t The data
 */
extern uint8_t can_fetch_byte(uint32_t id, uint8_t byte);

extern uint8_t can_fetch_len(uint32_t id);

extern uint32_t can_fetch_timestamp(uint32_t id);

extern bool can_check_timestamp(uint32_t id);


extern void can_send_msg(can_msg_t tx_msg);


