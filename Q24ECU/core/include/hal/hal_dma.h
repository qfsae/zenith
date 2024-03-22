/**
 * @file hal_dma.h
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2024-02-25
 * 
 * Direct Memory Access Hardware Abstraction Layer
 *
 * This is a relatively basic implementation of the DMA that can be used to
 * transfer memory from a fixed 
 *
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "stm32f4xx.h"
#include <inttypes.h>
#include "errors.h"
#include <stddef.h>

enum DMA_CHANNEL {
    DMA_CHANNEL_0 = 0,
    DMA_CHANNEL_1 = 1,
    DMA_CHANNEL_2 = 2,
    DMA_CHANNEL_3 = 3,
    DMA_CHANNEL_4 = 4,
    DMA_CHANNEL_5 = 5,
    DMA_CHANNEL_6 = 6,
    DMA_CHANNEL_7 = 7
};

enum DMA_PRIORITY {
    DMA_PRIORITY_LOW = 0,
    DMA_PRIORITY_MEDIUM = 1,
    DMA_PRIORITY_HIGH = 2,
    DMA_PRIORITY_VERY_HIGH = 3
};

enum DMA_MEM_SIZE {
    DMA_MEM_SIZE_8 = 0,
    DMA_MEM_SIZE_16 = 1,
    DMA_MEM_SIZE_32 = 2
};

/**
 * @brief Initialize the DMA peripheral
 * 
 * @param dma DMA peripheral to initialize
 * @param stream DMA Stream to initialize
 * @param ch DMA Channel to use
 * @param priority DMA Priority Level
 * @param mem_size DMA Memory Size
 * @param periph_addr Peripheral Address
 * @param mem_addr Memory Address
 */


static inline enum SYS_ERROR hal_dma_init(DMA_TypeDef *dma, DMA_Stream_TypeDef *stream, enum DMA_CHANNEL ch, enum DMA_PRIORITY priority, enum DMA_MEM_SIZE mem_size, volatile void *periph_addr, void *mem_addr, size_t num_transfers) {
    // Enable DMA Clock
    if (dma == DMA1) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    } else if (dma == DMA2) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    } else {
        return SYS_INVALID_ARG;
    }

    /**             DMA Stream Setup            **/

    // Disable Stream
    CLEAR_BIT(stream->CR, DMA_SxCR_EN);
    // Set Channel
    CLEAR_BIT(stream->CR, DMA_SxCR_CHSEL);
    SET_BIT(stream->CR, (uint32_t)(ch << DMA_SxCR_CHSEL_Pos));

    // Set Peripheral Data Size
    CLEAR_BIT(stream->CR, DMA_SxCR_PSIZE);
    SET_BIT(stream->CR, (uint32_t)(mem_size << DMA_SxCR_PSIZE_Pos));

    // Set Priority Level
    CLEAR_BIT(stream->CR, DMA_SxCR_PL);
    SET_BIT(stream->CR, (uint32_t)(priority << DMA_SxCR_PL_Pos));

    // Setup Memory Increments (Peripheral Increment is disabled, Memory Increment is enabled)
    SET_BIT(stream->CR, DMA_SxCR_MINC);
    CLEAR_BIT(stream->CR, DMA_SxCR_PINC);
    // Setup Circular Mode
    SET_BIT(stream->CR, DMA_SxCR_CIRC);
    // Set Direct Write to Memory (no FIFO)
    CLEAR_BIT(DMA2_Stream0->FCR, DMA_SxFCR_DMDIS);

    // Setup Transfers
    DMA2_Stream0->NDTR = (uint32_t)num_transfers;
    DMA2_Stream0->PAR = (uint32_t)(periph_addr);
    DMA2_Stream0->M0AR = (uint32_t)(mem_addr);

    return SYS_OK;
}

/**
 * @brief Start the DMA Stream
 * 
 * @param stream DMA Stream to start
 */
static inline void hal_dma_start(DMA_Stream_TypeDef *stream) {
    SET_BIT(stream->CR, DMA_SxCR_EN);
}

/**
 * @brief Stop the DMA Stream
 * 
 * @param stream DMA Stream to stop
 */
static inline void hal_dma_stop(DMA_Stream_TypeDef *stream) {
    CLEAR_BIT(stream->CR, DMA_SxCR_EN);
}


