/**
 * @file hal_spi.h
 * @author Josh Allen
 * @brief
 * @version 0.1
 * @date 2023-12-02
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <stm32f446xx.h>

// Generic Macros - may move somewhere else
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define FLAG_RESET RESET
#define FLAG_SET SET

// Clock macros -- check if this is correct for this MCU
#define SPI1_PCLK_EN() (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN() (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN() (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN() (RCC->APB2ENR |= (1 << 13))

#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI() (RCC->APB2ENR &= ~(1 << 13))

// Register Reset - TODO: check if the right registers were used
#define SPI1_REG_RESET()               \
    do                                 \
    {                                  \
        (RCC->APB2RSTR |= (1 << 12));  \
        (RCC->APB2RSTR &= ~(1 << 12)); \
    } while (0)

#define SPI2_REG_RESET()               \
    do                                 \
    {                                  \
        (RCC->APB1RSTR |= (1 << 14));  \
        (RCC->APB1RSTR &= ~(1 << 14)); \
    } while (0)

#define SPI3_REG_RESET()               \
    do                                 \
    {                                  \
        (RCC->APB1RSTR |= (1 << 15));  \
        (RCC->APB1RSTR &= ~(1 << 15)); \
    } while (0)

#define SPI4_REG_RESET()               \
    do                                 \
    {                                  \
        (RCC->APB2RSTR |= (1 << 13));  \
        (RCC->APB2RSTR &= ~(1 << 13)); \
    } while (0)

// SPI_DeviceMode
#define SPI_DEVICE_MODE_SLAVE 0
#define SPI_DEVICE_MODE_MASTER 1
// SPI_BusConfig
#define SPI_BUS_CONFIG_FD 1
#define SPI_BUS_CONFIG_HD 2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY 3
// SPI_SclkSpeed
#define SPI_SCLK_SPEED_DIV2 0
#define SPI_SCLK_SPEED_DIV4 1
#define SPI_SCLK_SPEED_DIV8 2
#define SPI_SCLK_SPEED_DIV16 3
#define SPI_SCLK_SPEED_DIV32 4
#define SPI_SCLK_SPEED_DIV64 5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7
// SPI_DFF
#define SPI_DFF_8BITS 0
#define SPI_DFF_16BITS 1
// SPI_CPOL
#define SPI_CPOL_LOW 0
#define SPI_CPOL_HIGH 1
// SPI_CPHA
#define SPI_CPHA_LOW 0
#define SPI_CPHA_HIGH 1
// SPI_SSM
#define SPI_SSM_DI 0
#define SPI_SSM_EN 1

// SPI Application States - used for interrupts mostly
#define SPI_READY 0
#define SPI_BUSY_IN_RX 1
#define SPI_BUSY_IN_TX 2

// Possible SPI application events
#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR 3
#define SPI_EVENT_CRC_ERR 4
