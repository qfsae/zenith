/**
 * @file cm200dx.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @author Matt Dobaj
 * @brief Cascadia CM200DX Inverter - CAN Bus Interface
 * @version 0.1
 * @date 2024-03-14
 * 
 * Example code for an Interface
 *
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "drivers/can.h"

enum CM200DX {
    LEFT = 0x100,
    RIGHT = 0x200
};


static inline uint16_t cm200dx_getRPM(enum CM200DX baseID){
    can_msg_t msg =  can_read(CAN1, baseID);
    return (uint16_t)(msg.data[0] | msg.data[1]);
}


static inline uint16_t cm200dx_getPower(enum CM200DX baseID){
    return (uint16_t)(100U*can_read(CAN1, baseID).data[3]);
}

static inline void cm200dx_setPower(enum CM200DX baseID, uint8_t power){
    can_msg_t msg;
    msg.id =  baseID+1UL;
    msg.len = 1;
    msg.data[0] = power;
    can_write(CAN1, &msg, 10U);
}


