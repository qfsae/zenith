/**
 * @file hal_can.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io) Ethan Peterson (https://portfolio.petetech.net)
 * @brief STM32F4 CAN Hardware Abstraction Layer (Adaptation of Ethan Peterson's STM32F4 CAN library for Arduino HAL)
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "stm32f4xx.h"
#include "hal_gpio.h"
#include "pins.h"

#define CAN_EXT_ID_MASK                 0x1FFFFFFFU
#define CAN_STD_ID_MASK                 0x000007FFU

typedef struct {
    uint32_t id;        /* 29 bit identifier                               */
    uint8_t  data[8];   /* Data field                                      */
    uint8_t  len;       /* Length of data field in bytes                   */
    uint8_t  ch;        /* Object channel(Not use)                         */
    uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
    uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

/* Symbolic names for bit rate of CAN message                                */
typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

typedef const struct
{
    uint8_t TS2;
    uint8_t TS1;
    uint8_t BRP;
} CAN_bit_timing_config_t;


static inline void hal_can_init(CAN_TypeDef * CAN, BITRATE bitrate){
    CAN_bit_timing_config_t can_configs[6] = {{2, 12, 60}, {2, 12, 30}, {2, 12, 24}, {2, 12, 12}, {2, 12, 6}, {1, 7, 5}};
    if(CAN == CAN1){
        RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    }else if(CAN == CAN2){
        RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
    }

    // Request initialization
    SET_BIT(CAN->MCR, CAN_MCR_INRQ);
    while((CAN->MSR & CAN_MSR_INAK) == 0U) asm("nop");

    // Exit from sleep mode
    CLEAR_BIT(CAN->MCR, CAN_MCR_SLEEP);
    while((CAN->MSR & CAN_MSR_SLAK) != 0U) asm("nop");

    // Disable time triggered communication mode
    CLEAR_BIT(CAN->MCR, CAN_MCR_TTCM);
    // Disable automatic bus-off management
    CLEAR_BIT(CAN->MCR, CAN_MCR_ABOM);
    // Disable automatic wakeup mode
    CLEAR_BIT(CAN->MCR, CAN_MCR_AWUM);

    // Enable Automatic retransmission based on Configuration File Defines
    #ifdef USE_CAN_AUTOMATIC_RETRANSMISSION
    CLEAR_BIT(CAN->MCR, CAN_MCR_NART);
    #else
    SET_BIT(CAN->MCR, CAN_MCR_NART);
    #endif

    // Disable receive FIFO locked mode
    CLEAR_BIT(CAN->MCR, CAN_MCR_RFLM);
    // Disable transmit FIFO priority
    CLEAR_BIT(CAN->MCR, CAN_MCR_TXFP);
    // Set bit timing
    CAN->BTR &= ~(((0x03) << 24) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FF)); 
    CAN->BTR |=  (((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF);

    // Leave initialization
    CLEAR_BIT(CAN->MCR, CAN_MCR_INRQ);
    while((CAN->MSR & CAN_MSR_INAK) != 0U) asm("nop");
}

static inline void hal_can_receive(CAN_TypeDef * CAN, CAN_msg_t * rx_msg){
    // Determine Message ID format
    uint32_t id = CAN->sFIFOMailBox[0].RIR;
    if((id & (1U << 2U)) == 0){
        rx_msg->format = STANDARD_FORMAT;
        rx_msg->id = (CAN_STD_ID_MASK & (id >> 21U));
    }
    else{
        rx_msg->format = EXTENDED_FORMAT;
        rx_msg->id = (CAN_EXT_ID_MASK & (id >> 3U));
    }

    if((id & (1U << 1U)) == 0){
        rx_msg->type = DATA_FRAME;
    }
    else{
        rx_msg->type = REMOTE_FRAME;
    }

    // Message data length
    rx_msg->len = (CAN->sFIFOMailBox[0].RDTR) & 0xFUL;

    rx_msg->data[0] = 0xFFUL &  CAN->sFIFOMailBox[0].RDLR;
    rx_msg->data[1] = 0xFFUL & (CAN->sFIFOMailBox[0].RDLR >> 8);
    rx_msg->data[2] = 0xFFUL & (CAN->sFIFOMailBox[0].RDLR >> 16);
    rx_msg->data[3] = 0xFFUL & (CAN->sFIFOMailBox[0].RDLR >> 24);
    rx_msg->data[4] = 0xFFUL &  CAN->sFIFOMailBox[0].RDHR;
    rx_msg->data[5] = 0xFFUL & (CAN->sFIFOMailBox[0].RDHR >> 8);
    rx_msg->data[6] = 0xFFUL & (CAN->sFIFOMailBox[0].RDHR >> 16);
    rx_msg->data[7] = 0xFFUL & (CAN->sFIFOMailBox[0].RDHR >> 24);

    CAN->RF0R |= CAN_RF0R_RFOM0;
    
}

static inline bool CANMsgAvail(CAN_TypeDef * CAN)
{
    // Check for pending FIFO 0 messages
    return CAN->RF0R & 0x3UL;
}