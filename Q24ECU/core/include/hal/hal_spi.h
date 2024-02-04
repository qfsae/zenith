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

// Handle structure for SPIx peripheral
typedef struct
{
    SPI_TypeDef *pSPIx; // Holds the base address of the SPI peripheral. Struct overlay for the SPI registers.
    SPI_Config_t SPIConfig;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint8_t TxState;
    uint8_t RxState;
} SPI_Handle_t;

// Peripheral clocks setup
static inline void SPI_PeriClockControl(SPI_TypeDef *pSPIx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
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
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
    // First configure SPI_CR1 register
    uint32_t tempReg = 0;

    // 1. Configure device mode
    tempReg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR_Pos;

    // 2. Configure bus config
    if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
    {
        // BIDI mode should be cleared
        tempReg &= ~(1 << SPI_CR1_BIDIMODE_Pos);
    }
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
    {
        // BIDI mode should be set
        tempReg |= (1 << SPI_CR1_BIDIMODE_Pos);
    }
    else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
    {
        // BIDI mode should be cleared
        tempReg &= ~(1 << SPI_CR1_BIDIMODE_Pos);
        // RXONLY bit must be set
        tempReg |= (1 << SPI_CR1_RXONLY_Pos);
    }

    // 3. Configure the SPI serial clock speed (baud rate)
    tempReg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR_Pos;

    // 4. Configure the DFF
    tempReg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF_Pos;

    // 5. Configure the polarity
    tempReg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL_Pos;

    // 6. Configure the phase
    tempReg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA_Pos;

    pSPIHandle->pSPIx->CR1 = tempReg;
}

void SPI_DeInit(SPI_TypeDef *pSPIx)
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
void SPI_SendData(SPI_TypeDef *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // 1. Wait until TXE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_SR_TXE) == FLAG_RESET)
            ;

        // 2. Check the DFF bit in CR1
        if ((pSPIx->CR1 & (1 << SPI_CR1_DFF_Pos)))
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

void SPI_ReceiveData(SPI_TypeDef *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
    while (Len > 0)
    {
        // 1. Wait until RXNE is set
        while (SPI_GetFlagStatus(pSPIx, SPI_SR_RXNE) == FLAG_RESET)
            ;

        // 2. Check the DFF bit in CR1
        if ((pSPIx->CR1 & (1 << SPI_CR1_DFF_Pos)))
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

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
    pSPIHandle->TxLen = 0;
    pSPIHandle->pTxBuffer = NULL;
    pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
    pSPIHandle->RxLen = 0;
    pSPIHandle->pRxBuffer = NULL;
    pSPIHandle->RxState = SPI_READY;
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t appEvent)
{

    // This is a weak implementation. The application may override this function.
}

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    // Check DFF in CR1
    if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
    {
        // 16 bit dff
        pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);
        pSPIHandle->TxLen -= 2;
        (uint16_t *)pSPIHandle->pTxBuffer++;
    }
    else
    {
        // 8 bit dff
        pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
        pSPIHandle->TxLen--;
        pSPIHandle->pTxBuffer++;
    }

    if (!pSPIHandle->TxLen)
        // TxLen is zero, so close the spi communication
        // and inform app that TX is over
        // this prevents interrupts from setting up of TXE flag
        SPI_CloseTransmission(pSPIHandle);

    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    // Check DFF in CR1
    if (pSPIHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF))
    {
        // 16 bit dff
        *((uint16_t *)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPIx->DR;
        pSPIHandle->RxLen -= 2;
        (uint16_t *)pSPIHandle->pTxBuffer++;
    }
    else
    {
        // 8 bit dff
        *(pSPIHandle->pRxBuffer) = (uint8_t)pSPIHandle->pSPIx->DR;
        pSPIHandle->RxLen--;
        pSPIHandle->pRxBuffer++;
    }

    if (!pSPIHandle->RxLen)
    {
        // TxLen is zero, so close the spi communication
        // and inform app that TX is over
        // this precents interrupts from setting up of TXE flag
        SPI_CloseReception(pSPIHandle);

        SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
    }
}
