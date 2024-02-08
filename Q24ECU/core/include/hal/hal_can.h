/**
 * @file hal_can.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io/)
 * @author Ethan Peterson (https://portfolio.petetech.net/)
 * @brief STM32F4 CAN Hardware Abstraction Layer (Adaptation of Ethan Peterson's STM32F4 CAN library for Arduino HAL)
 * @version 0.1
 * @date 2023-12-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "stm32f4xx.h"
#include "errors.h"
#include "hal_gpio.h"

typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
    uint8_t format;
    uint8_t type;
    uint32_t timestamp;
} can_msg_t;


/* Symbolic names for bit rate of CAN message                                */
typedef enum can_bitrate {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} CAN_BITRATE;

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

// CAN bus timing configurations
static CAN_bit_timing_config_t can_configs[6] = {{2, 12, 60}, {2, 12, 30}, {2, 12, 24}, {2, 12, 12}, {2, 12, 6}, {1, 7, 5}};


/**
 * @brief Set a Filter
 * 
 * @param index Filter Index (CAN1 = 0-13 | CAN2 = 14-28)
 * @param scale   - Select filter scale.
 *                    0: Dual 16-bit scale configuration
 *                    1: Single 32-bit scale configuration
 * @param mode    - Select filter mode.
 *                    0: Two 32-bit registers of filter bank x are in Identifier Mask mode
 *                    1: Two 32-bit registers of filter bank x are in Identifier List mode
 * @param fifo    - Select filter assigned.
 *                    0: Filter assigned to FIFO 0
 *                    1: Filter assigned to FIFO 1
 * @param bank1   - Filter bank register 1
 * @param bank2   - Filter bank register 2
 *
 * @returns HAL_CAN_OK or HAL_CAN_xx_ERR on ERROR
 */
static inline uint8_t hal_can_setFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2) {
    if (index > 27)
        return HAL_CAN_FILTER_SELRNG_ERR;

    CLEAR_BIT(CAN1->FA1R, (0x1UL << index)); // Deactivate filter

    if (scale == 0)
    {
        CAN1->FS1R &= ~(0x1UL << index); // Set filter to Dual 16-bit scale configuration
    }
    else
    {
        CAN1->FS1R |= (0x1UL << index); // Set filter to single 32 bit configuration
    }
    if (mode == 0)
    {
        CAN1->FM1R &= ~(0x1UL << index); // Set filter to Mask mode
    }
    else
    {
        CAN1->FM1R |= (0x1UL << index); // Set filter to List mode
    }

    if (fifo == 0)
    {
        CAN1->FFA1R &= ~(0x1UL << index); // Set filter assigned to FIFO 0
    }
    else
    {
        CAN1->FFA1R |= (0x1UL << index); // Set filter assigned to FIFO 1
    }

    CAN1->sFilterRegister[index].FR1 = bank1; // Set filter bank registers1
    CAN1->sFilterRegister[index].FR2 = bank2; // Set filter bank registers2

    SET_BIT(CAN1->FA1R, (0x1UL << index)); // Reactivate the filter

    return SYS_OK;
}

/**
 * @brief Initialize a CAN bus
 * 
 * @param CAN The CAN bus to initialize
 * @param bitrate CAN Bitrate (50, 100, 125, 250, 500, 1000 kbps)
 * @param NART TRUE to disable automatic retransmission. Should be FALSE for normal operation
 * @param pin_tx GPIO PIN (automatic setup for can bus)
 * @param pin_rx GPIO PIN (automatic setup for can bus)
 * @returns HAL_CAN_OK or HAL_CAN_xx_ERR upon failure
 */
static inline uint8_t hal_can_init(CAN_TypeDef * CAN, CAN_BITRATE bitrate, bool NART, uint16_t pin_tx, uint16_t pin_rx){
    // Enable the CAN bus clock
    if(CAN == CAN1) RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    if(CAN == CAN2) RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
    // Set up the GPIO pins
    gpio_set_mode(pin_tx, GPIO_MODE_AF);
    gpio_set_af(pin_tx, GPIO_AF_CAN); // GPIO Alternate function 9 -> CAN Bus
    gpio_set_mode(pin_rx, GPIO_MODE_AF);
    gpio_set_af(pin_rx, GPIO_AF_CAN);

    // Request the CAN bus to enter into initialization mode
    SET_BIT(CAN->MCR, CAN_MCR_INRQ);
    while(!(CAN->MSR & CAN_MSR_INAK));

    // Reset All other bits leaving INRQ
    CAN->MCR = 0x1UL; // NOTE: This assignment is meant to clear the register. Use of (=) operand is not a mistake

    // Enable automatic bus error management
    SET_BIT(CAN->MCR, CAN_MCR_ABOM);
    // Configure Automatic Retransmission
    if(NART) SET_BIT(CAN->MCR, CAN_MCR_NART);
    // Disable Time Controlled Communication Mode (extension of CAN protocol)
    CLEAR_BIT(CAN->MCR, CAN_MCR_TTCM);
    // Enable FIFO mode for tx registers
    SET_BIT(CAN->MCR, CAN_MCR_TXFP);
    // Setup bus bitrates
    CAN->BTR &= ~((0x03UL << 24) | (0x07UL << 20) | (0x0FUL << 16) | (0x1FFUL)); // Zero out the register
    CAN->BTR |=  (uint32_t)((((can_configs[bitrate].TS2-1) & 0x07) << 20) | (((can_configs[bitrate].TS1-1) & 0x0F) << 16) | ((can_configs[bitrate].BRP-1) & 0x1FF)); // Set up the bit timing
    // Initialize the filter registers
    SET_BIT(CAN1->FMR, CAN_FMR_FINIT);           // Enter into initialization mode
    CLEAR_BIT(CAN1->FMR, CAN_FMR_CAN2SB);        // Clear the Filter Selection register
    CAN1->FMR |= (0xEUL << CAN_FMR_CAN2SB_Pos); // Set filters (0-13 -> CAN1) & (14-28 -> CAN2)
    uint8_t f1_status = hal_can_setFilter(0, 1, 0, 0, 0x0UL, 0x0UL);
    uint8_t f2_status = hal_can_setFilter(14, 1, 0, 0, 0x0UL, 0x0UL);
    CLEAR_BIT(CAN1->FMR, CAN_FMR_FINIT);              // Deactivate initialization mode

    if(f1_status != SYS_OK) return f1_status;
    if(f2_status != SYS_OK) return f2_status;

    // Enable the bus
    // Request to leave initialization mode
    CAN->MCR &= ~(CAN_MCR_INRQ);
    uint32_t timeout = 9999;
    for(uint32_t wait_ack = 0; wait_ack < timeout; wait_ack++){
        if((CAN->MSR & CAN_MSR_INAK) == 0){
            // Return: Success if CAN enables successfully
            return SYS_OK;
        }
        for(uint32_t spin = 0; spin < timeout; spin++);
    }
    // Return error if CAN not initialized by end of timeout
    return HAL_CAN_INIT_ERR;
}

static inline void hal_can_read(CAN_TypeDef * CAN, can_msg_t * rx_msg){
    // Determine Message ID format (Identifier Extension)
    rx_msg->format = (CAN_RI0R_IDE & CAN->sFIFOMailBox[0].RIR);
    if(rx_msg->format == STANDARD_FORMAT){
        rx_msg->id = (CAN_RI0R_STID & CAN->sFIFOMailBox[0].RIR) >> CAN_TI0R_STID_Pos;
    }
    else{
        rx_msg->id = ((CAN_RI0R_EXID | CAN_RI0R_STID) & CAN->sFIFOMailBox[0].RIR) >> CAN_RI0R_EXID_Pos;
    }

    // Data Frame = 0 | Remote Frame = 1
    rx_msg->type = (CAN_RI0R_RTR & CAN->sFIFOMailBox[0].RIR);

    // Message data length
    rx_msg->len = (CAN_RDT0R_DLC & CAN->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_DLC_Pos;

    // Unload the data
    rx_msg->data[0] = (uint8_t)((CAN_RDL0R_DATA0 & CAN->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA0_Pos);
    rx_msg->data[1] = (uint8_t)((CAN_RDL0R_DATA1 & CAN->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA1_Pos);
    rx_msg->data[2] = (uint8_t)((CAN_RDL0R_DATA2 & CAN->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA2_Pos);
    rx_msg->data[3] = (uint8_t)((CAN_RDL0R_DATA3 & CAN->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA3_Pos);
    rx_msg->data[4] = (uint8_t)((CAN_RDH0R_DATA4 & CAN->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA4_Pos);
    rx_msg->data[5] = (uint8_t)((CAN_RDH0R_DATA5 & CAN->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA5_Pos);
    rx_msg->data[6] = (uint8_t)((CAN_RDH0R_DATA6 & CAN->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA6_Pos);
    rx_msg->data[7] = (uint8_t)((CAN_RDH0R_DATA7 & CAN->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA7_Pos);

    // Release the mailbox to hardware control
    SET_BIT(CAN->RF0R, CAN_RF0R_RFOM0);
}

/**
 * @brief Get if a CAN message is pending in the CAN bus FIFO
 * 
 * @param CAN The CAN bus FIFO to check
 * @returns TRUE if a message is pending
 * @returns FALSE if the FIFO is empty
 */
static inline bool hal_can_read_ready(CAN_TypeDef * CAN)
{
    // Check for pending FIFO 0 messages
    return CAN->RF0R & 0x3UL;
}


/**
 * @brief Send a CAN message. Must wait for message to send before attempting another transmission
 * 
 * @param CAN the CAN bus to send on
 * @param tx_msg pointer to the message to send
 * @param mailbox The TX mailbox to use (0..2). Use 0 as default
 * @return uint8_t HAL_CAN_OK or HAL_CAN_xx_ERR on on error
 */
static inline uint8_t hal_can_send(CAN_TypeDef * CAN, can_msg_t * tx_msg, uint8_t mailbox) {
    // Check the mailbox is empty before attempting to send
    if(CAN->sTxMailBox[mailbox].TIR & CAN_TI0R_TXRQ_Msk) return HAL_CAN_MAILBOX_NONEMPTY;
    if(mailbox > 2) return HAL_CAN_MAILBOX_SELRNG_ERR;

    // Create temp variable to store mailbox info
    uint32_t sTxMailBox_TIR = 0;
    if(tx_msg->format == EXTENDED_FORMAT){
        // Extended msg frame format
        sTxMailBox_TIR = (tx_msg->id << CAN_TI0R_EXID_Pos) | CAN_TI0R_IDE;
    }
    else{
        // Standard msg frame format
        sTxMailBox_TIR = (tx_msg->id << CAN_TI0R_STID_Pos);
    }
    
    // Remote frame
    if(tx_msg->type == REMOTE_FRAME){
        SET_BIT(sTxMailBox_TIR, CAN_TI0R_RTR);
    }

    // Clear and set the message length
    CLEAR_BIT(CAN->sTxMailBox[mailbox].TDTR, CAN_TDT0R_DLC);
    SET_BIT(CAN->sTxMailBox[mailbox].TDTR, (tx_msg->len & CAN_TDT0R_DLC));

    // Load the DR's
    CAN->sTxMailBox[mailbox].TDLR = (((uint32_t) tx_msg->data[3] << CAN_TDL0R_DATA3_Pos) | ((uint32_t) tx_msg->data[2] << CAN_TDL0R_DATA2_Pos) | ((uint32_t) tx_msg->data[1] << CAN_TDL0R_DATA1_Pos) | ((uint32_t) tx_msg->data[0] << CAN_TDL0R_DATA0_Pos));
    CAN->sTxMailBox[mailbox].TDHR = (((uint32_t) tx_msg->data[7] << CAN_TDH0R_DATA7_Pos) | ((uint32_t) tx_msg->data[6] << CAN_TDH0R_DATA6_Pos) | ((uint32_t) tx_msg->data[5] << CAN_TDH0R_DATA5_Pos) | ((uint32_t) tx_msg->data[4] << CAN_TDH0R_DATA4_Pos));

    CAN->sTxMailBox[mailbox].TIR = (uint32_t)(sTxMailBox_TIR | CAN_TI0R_TXRQ);
    
    // Return read OK
    return SYS_OK;

}

/**
 * @brief Get is the Transmit Mailbox is empty
 * @param CAN The CAN bus mailbox to check
 * @param mailbox The TX mailbox to use (0..2). Use 0 as default
 * @return true if the mailbox is empty
 * @return false if the mailbox is pending
 */
static inline bool hal_can_send_ready(CAN_TypeDef * CAN, uint8_t mailbox){
    // Check to see if mailbox is empty
    return !(CAN->sTxMailBox[mailbox].TIR & CAN_TI0R_TXRQ_Msk);
}


