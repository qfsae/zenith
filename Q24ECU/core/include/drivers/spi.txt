
/**
 * @file interface_spi.h
 * @author Josh Allen
 * @brief
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "hal/hal_spi.h"
#include <stm32f446xx.h>

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

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~SPI_CR2_TXEIE;
    pSPIHandle->TxLen = 0;
    pSPIHandle->pTxBuffer = NULL;
    pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
    pSPIHandle->pSPIx->CR2 &= ~SPI_CR2_RXNEIE;
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
    if (pSPIHandle->pSPIx->CR1 & SPI_CR1_DFF)
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
    if (pSPIHandle->pSPIx->CR1 & SPI_CR1_DFF)
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

static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
    uint8_t temp;
    // 1. clear the OVR flag
    if (pSPIHandle->TxState != SPI_BUSY_IN_TX)
    {
        temp = pSPIHandle->pSPIx->DR;
        temp = pSPIHandle->pSPIx->SR;
    }
    (void)temp;
    // If the SPI is busy in TX, then the error will not be cleared and the application will have to clear bit on own
    // In this case, application should call SPI_ClearOVRFlag
    // 2. inform the application
    SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
    uint8_t status, control; // change these?

    // first check for TXE
    status = pSPIHandle->pSPIx->SR & SPI_SR_TXE;      // will be 1 if TXE is set
    control = pSPIHandle->pSPIx->CR2 & SPI_CR2_TXEIE; // will be 1 if TXEIE is set

    if (status && control)
    {
        // handle TXE
        spi_txe_interrupt_handle(pSPIHandle);
    }

    // check for RXNE
    status = pSPIHandle->pSPIx->SR & SPI_SR_RXNE;      // will be 1 if RXNE is set
    control = pSPIHandle->pSPIx->CR2 & SPI_CR2_RXNEIE; // will be 1 if RXNEIE is set

    if (status && control)
    {
        // handle TXE
        spi_rxne_interrupt_handle(pSPIHandle);
    }

    // check for Overun flag
    status = pSPIHandle->pSPIx->SR & SPI_SR_OVR;      // will be 1 if OVR is set
    control = pSPIHandle->pSPIx->CR2 & SPI_CR2_ERRIE; // will be 1 if ERRIE is set

    if (status && control)
    {
        // handle TXE
        spi_ovr_err_interrupt_handle(pSPIHandle);
    }
}

uint8_t SPI_SendData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{
    uint8_t state = pSPIHandle->TxState;
    if (state != SPI_BUSY_IN_TX) // check this if state that it's TX and not RX
    {
        // 1. Save the Tx buffer address and Len information in some global variables
        pSPIHandle->pTxBuffer = pTxBuffer;
        pSPIHandle->TxLen = Len;

        // 2. Mark the SPI state as busy in transmission so that no other code can
        // take over the same SPI peripheral until transmission is over
        pSPIHandle->TxState = SPI_BUSY_IN_TX;

        // 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
        hal_spi_enable_txe(pSPIHandle->pSPIx, true);
    }
    return state;
}

uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
    uint8_t state = pSPIHandle->RxState;
    if (state != SPI_BUSY_IN_RX) // check this if state that it's TX and not RX
    {
        // 1. Save the Rx buffer address and Len information in some global variables
        pSPIHandle->pRxBuffer = pRxBuffer;
        pSPIHandle->TxLen = Len;

        // 2. Mark the SPI state as busy in transmission so that no other code can
        // take over the same SPI peripheral until transmission is over
        pSPIHandle->RxState = SPI_BUSY_IN_RX;

        // 3. Enable the RXNEIE control bit to get interrupt whenever RXNE flag is set in SR
        hal_spi_enable_rxne(pSPIHandle->pSPIx, true);
    }
    return state;
}