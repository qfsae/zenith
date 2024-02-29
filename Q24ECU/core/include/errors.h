/**
 * @file errors.h
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Global definitions for all possible system errors
 * @version 0.1
 * @date 2024-2-6
 * 
 * @copyright Copyright (c) 2024
 * 
 * 
 * The purpose of this file is to consolodate all possible system
 * errors in a single file. The number displayed on the two digit
 * display corrisoponds directly to the errors in this file. 
 *
 * Please allow error numbers to be automatically assigned as to
 * avoid conflicts. Also include a brief description of each error.
 *
 */

#pragma once

enum SYS_ERROR {
    SYS_OK = 0,
    // An invalid parameter was passed to a function
    SYS_INVALID_ARG,
    // Task has decided to shut down
    TASK_SHUTDOWN,
    // A Task has encountered a critical error and the system must shut down
    TASK_SHUTDOWN_CRITICAL,
    // Invalid Flash Address given
    FLASH_WRITE_ADDR_INVALID,
    // Failed to assert write permissions to the flash memory
    FLASH_WRITE_NOACC,
    // CAN1 must be enabled before CAN2
    HAL_CAN1_UNINIT_ERR,
    // There was an error initializing the CAN bus
    HAL_CAN_INIT_ERR,
    // Filter selection out of range (there are only 28 filters)
    HAL_CAN_FILTER_SELRNG_ERR,// Filter Out of range
    // Mailbox Not Empty
    HAL_CAN_MAILBOX_NONEMPTY,
    // Mailbox selection out of range
    HAL_CAN_MAILBOX_SELRNG_ERR,
    // Fatal Error on bus
    HAL_CAN_FATAL_ERR,
    // The UART port is not initialized
    UART_UNINIT_ERR,
    // The UART Semaphore could not be taken
    UART_ACC_ERR,
    // ADC Overflow (ADC is running faster than the system can monitor it)
    ADC_OVERFLOW,

    // Maximum Error Number
    SYS_ERROR_MAX // 99
};
