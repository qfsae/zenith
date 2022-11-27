#include "Arduino.h"

#define DEBUG 0

/* Symbolic names for bit rate of CAN message                                */
typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} BITRATE;

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME = 0, REMOTE_FRAME}         CAN_FRAME;

typedef struct
{
  uint32_t id;        /* 29 bit identifier                               */
  uint8_t  data[8];   /* Data field                                      */
  uint8_t  len;       /* Length of data field in bytes                   */
  uint8_t  ch;        /* Object channel(Not use)                         */
  uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

typedef const struct
{
  uint8_t TS2;
  uint8_t TS1;
  uint8_t BRP;
} CAN_bit_timing_config_t;

CAN_bit_timing_config_t can_configs[6] = {{2, 12, 60}, {2, 12, 30}, {2, 12, 24}, {2, 12, 12}, {2, 12, 6}, {1, 7, 5}};

/**
 * Initializes the CAN GPIO registers.
 *
 * @param addr    - Specified GPIO register address.
 * @param index   - Specified GPIO index.
 * @param speed   - Specified OSPEEDR register value.(Optional)
 *
 */
extern void CANSetGpio(GPIO_TypeDef *addr, uint8_t index, uint8_t speed = 3);

/**
 * Initializes the CAN filter registers.
 *
 * @preconditions   - This register can be written only when the filter initialization mode is set (FINIT=1) in the CAN_FMR register.
 * @param index   - Specified filter index. index 27:14 are available in connectivity line devices only.
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
 */
extern void CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint8_t bank1, uint8_t bank2);

/**
 * Initializes the CAN controller with specified bit rate.
 *
 * @param bitrate - Specified bitrate. If this value is not one of the defined constants, bit rate will be defaulted to 125KBS
 * @param _CAN1   - Select CAN 1 port
 * 
 *                    =0: CAN1_RX mapped to PA11, CAN1_TX mapped to PA12
 * 
 *                    =1: Not used
 * 
 *                    =2: CAN1_RX mapped to PB8,  CAN1_TX mapped to PB9 (not available on 36-pin package)
 * 
 *                    =3: CAN1_RX mapped to PD0,  CAN1_TX mapped to PD1 (available on 100-pin and 144-pin package)
 * 
 * @param _CAN2   - Select CAN 2 port
 * 
 *                    =0: CAN2_RX mapped to PB5 , CAN2_TX mapped to PB6
 * 
 *                    =1: Not used
 * 
 *                    =2: CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 * 
 *                    =3: CAN2_RX mapped to PB12, CAN2_TX mapped to PB13
 *
 */
extern bool CANInit(BITRATE bitrate, int _CAN1, int _CAN2);

/**
 * Decodes CAN messages from the data registers and populates a 
 * CAN message struct with the data fields.
 * 
 * @preconditions     - A valid CAN message is received
 * @param CAN_rx_msg - CAN message structure for reception
 * 
 */
extern void CANReceive(uint8_t ch, CAN_msg_t* CAN_rx_msg);

/**
 * Encodes CAN messages using the CAN message struct and populates the 
 * data registers with the sent.
 * 
 * @params CAN_tx_msg - CAN message structure for transmission
 * 
 */
extern void CANSend(uint8_t ch, CAN_msg_t* CAN_tx_msg);

 /**
 * Returns whether there are CAN messages available.
 *
 * @returns If pending CAN messages are in the CAN controller
 *
 */
extern uint8_t CANMsgAvail(uint8_t ch);