/**
 * @file pins.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief QFSAE Board Map File
 * @version 0.1
 * @date 2023-11-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once


// Package a pin bank (U8) and pin number (U8) into single package (U16)
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
// Retrieve pin number (U8) from pin package (U16)
#define PINNO(pin) (pin & 255)
// Retrieve pin bank (U8) from pin package (U16)
#define PINBANK(pin) (pin >> 8)

/**
 * Define the board to import its pinmap in the project
 * Options:
 * - BOARD_MOCKECU
 * - BOARD_VCU_V1
 * 
 */
#define BOARD_VCU_V1


/**
 * @brief Pin definitions for Ethan Peterson's MockECU
 * 
 */
#if defined(BOARD_MOCKECU)
// debug LED for MockECU
#define debug_led1 ((uint16_t)(PIN('B', 0)))
// debug LED for MockECU
#define debug_led2 ((uint16_t)(PIN('B', 1)))
// Hall Effect Current Sensor
#define PIN_currentSensor ((uint16_t)(PIN('C', 0)))
// Vin Voltage Divider
#define PIN_voltageSensor ((uint16_t)(PIN('C', 1)))

#define PIN_CAN1_RX ((uint16_t)(PIN('A', 11)))
#define PIN_CAN1_TX ((uint16_t)(PIN('A', 12)))

#endif // BOARD_MOCKECU


/**
 * @brief Pin Mappings for Q24 EV VCU (Version 1)
 * 
 */
#if defined(BOARD_VCU_V1)

// CAN Pins
#define PIN_CAN1_RX ((uint16_t)(PIN('A', 11)))
#define PIN_CAN1_TX ((uint16_t)(PIN('A', 12)))

#define PIN_CAN2_RX ((uint16_t)(PIN('B', 12)))
#define PIN_CAN2_TX ((uint16_t)(PIN('B', 13)))

// UART Pins
#define PIN_USART1_TX ((uint16_t)(PIN('B', 6)))
#define PIN_USART1_RX ((uint16_t)(PIN('B', 7)))

#define PIN_USART2_TX ((uint16_t)(PIN('D', 5)))
#define PIN_USART2_RX ((uint16_t)(PIN('D', 6)))

#define PIN_UART4_TX ((uint16_t)(PIN('A', 0)))
#define PIN_UART4_RX ((uint16_t)(PIN('A', 1)))

#define PIN_UART5_TX ((uint16_t)(PIN('E', 7)))
#define PIN_UART5_RX ((uint16_t)(PIN('E', 8)))

// Analog Input Pins
#define PIN_A12Vin_1 ((uint16_t)(PIN('C', 0)))
#define PIN_A12Vin_2 ((uint16_t)(PIN('C', 1)))
#define PIN_A12Vin_3 ((uint16_t)(PIN('B', 1)))
#define PIN_A12Vin_4 ((uint16_t)(PIN('C', 3)))
#define PIN_A12Vin_5 ((uint16_t)(PIN('A', 3)))
#define PIN_A12Vin_6 ((uint16_t)(PIN('A', 2)))

#define PIN_A5Vin_1 ((uint16_t)(PIN('A', 4)))
#define PIN_A5Vin_2 ((uint16_t)(PIN('A', 5)))
#define PIN_A5Vin_3 ((uint16_t)(PIN('A', 7)))
#define PIN_A5Vin_4 ((uint16_t)(PIN('A', 7)))

// ADC Channels for Analog Inputs
#define PIN_A5Vin_1_ADCCH 4U
#define PIN_A5Vin_2_ADCCH 5U



#endif // BOARD_VCU_V1

