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
#define BOARD_MOCKECU


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

// Debug USART (for printf)
#define UART_DEBUG USART2


#endif // BOARD_MOCKECU


/**
 * @brief Pin Mappings for Q24 EV VCU (Version 1)
 * 
 */
#if defined(BOARD_VCU_V1)


#endif // BOARD_VCU_V1



