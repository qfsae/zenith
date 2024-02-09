/**
 * @file hal_spi.h
 * @author Josh Allen
 * @brief
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
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

// Clock macros
#define SPI1_PCLK_EN() (RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
#define SPI2_PCLK_EN() (RCC->APB1ENR |= RCC_APB1ENR_SPI2EN)
#define SPI3_PCLK_EN() (RCC->APB1ENR |= RCC_APB1ENR_SPI3EN)
#define SPI4_PCLK_EN() (RCC->APB2ENR |= RCC_APB2ENR_SPI4EN)

#define SPI1_PCLK_DI() (RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN)
#define SPI2_PCLK_DI() (RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN)
#define SPI3_PCLK_DI() (RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN)
#define SPI4_PCLK_DI() (RCC->APB2ENR &= ~RCC_APB2ENR_SPI4EN)

// Register Reset
#define SPI1_REG_RESET()                          \
    do                                            \
    {                                             \
        (RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST);  \
        (RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST); \
    } while (0)

#define SPI2_REG_RESET()                          \
    do                                            \
    {                                             \
        (RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST);  \
        (RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST); \
    } while (0)

#define SPI3_REG_RESET()                          \
    do                                            \
    {                                             \
        (RCC->APB1RSTR |= RCC_APB1RSTR_SPI3RST);  \
        (RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST); \
    } while (0)

#define SPI4_REG_RESET()                          \
    do                                            \
    {                                             \
        (RCC->APB2RSTR |= RCC_APB2RSTR_SPI4RST);  \
        (RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI4RST); \
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

// Configuration structure for SPIx peripheral
typedef struct
{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;  // Data frame format
    uint8_t SPI_CPOL; // Polarity
    uint8_t SPI_CPHA; // Phase
    uint8_t SPI_SSM;  // Slave Management (Software or Hardware)
} SPI_Config_t;

// Peripheral clocks setup
static inline void HAL_SPI_PeriClockControl(SPI_TypeDef *pSPIx, bool enable)
{
    if (enable)
    {
        if (pSPIx == SPI1)
        {
            SPI1_PCLK_EN();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLK_EN();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLK_EN();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_PCLK_EN();
        }
    }
    else
    {
        if (pSPIx == SPI1)
        {
            SPI1_PCLK_DI();
        }
        else if (pSPIx == SPI2)
        {
            SPI2_PCLK_DI();
        }
        else if (pSPIx == SPI3)
        {
            SPI3_PCLK_DI();
        }
        else if (pSPIx == SPI4)
        {
            SPI4_PCLK_DI();
        }
    }
}

// Init and DeInit
void HAL_SPI_Init(SPI_TypeDef *pSPIx, SPI_Config_t *pSPIConfig)
{
    // First configure SPI_CR1 register
    uint32_t tempReg = 0;

    // 1. Configure device mode
    tempReg |= pSPIConfig->SPI_DeviceMode << SPI_CR1_MSTR_Pos;

    // 2. Configure bus config
    if (pSPIConfig->SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        // BIDI mode should be cleared
        tempReg &= ~(SPI_CR1_BIDIMODE);
    }
    else if (pSPIConfig->SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        // BIDI mode should be set
        tempReg |= SPI_CR1_BIDIMODE;
    }
    else if (pSPIConfig->SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        // BIDI mode should be cleared
        tempReg &= ~(SPI_CR1_BIDIMODE);
        // RXONLY bit must be set
        tempReg |= SPI_CR1_RXONLY;
    }

    // 3. Configure the SPI serial clock speed (baud rate)
    tempReg |= pSPIConfig->SPI_SclkSpeed << SPI_CR1_BR_Pos;

    // 4. Configure the DFF
    tempReg |= pSPIConfig->SPI_DFF << SPI_CR1_DFF_Pos;

    // 5. Configure the polarity
    tempReg |= pSPIConfig->SPI_CPOL << SPI_CR1_CPOL_Pos;

    // 6. Configure the phase
    tempReg |= pSPIConfig->SPI_CPHA << SPI_CR1_CPHA_Pos;

    pSPIx->CR1 = tempReg;
}

void HAL_SPI_DeInit(SPI_TypeDef *pSPIx)
{
    if (pSPIx == SPI1)
    {
        SPI1_REG_RESET();
    }
    else if (pSPIx == SPI2)
    {
        SPI2_REG_RESET();
    }
    else if (pSPIx == SPI3)
    {
        SPI3_REG_RESET();
    }
    else if (pSPIx == SPI4)
    {
        SPI4_REG_RESET();
    }
}

// Data send and receive - blocking call
void HAL_SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // 1. Wait until TXE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_SR_TXE) == FLAG_RESET)
            ;

        // 2. Check the DFF bit in CR1
        if (pSPIx->CR1 & SPI_CR1_DFF)
        {
            // 16 bit DFF
            // Load the data
            pSPIx->DR = *((uint16_t *)pTxBuffer);
            Len -= 2;
            (uint16_t *)pTxBuffer++;
        }
        else
        {
            // 8 bit DFF
            // Load the data
            pSPIx->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
}

void HAL_SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // 1. Wait until RXNE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_SR_RXNE) == FLAG_RESET)
            ;

        // 2. Check the DFF bit in CR1
        if (pSPIx->CR1 & SPI_CR1_DFF_Pos)
        {
            // 16 bit DFF
            // Load the data
            *((uint16_t *)pRxBuffer) = pSPIx->DR;
            Len -= 2;
            (uint16_t *)pRxBuffer++;
        }
        else
        {
            // 8 bit DFF
            // Load the data
            *(pRxBuffer) = pSPIx->DR;
            Len--;
            pRxBuffer++;
        }
    }
}

uint8_t SPI_GetFlagStatus(SPI_TypeDef *pSPIx, uint32_t FlagName)
{
    if (pSPIx->SR & FlagName)
    {
        return FLAG_SET;
    }
    return FLAG_RESET;
}
