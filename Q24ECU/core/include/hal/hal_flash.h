/**
 * @file hal_flash.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Base Abstraction for interaction with STM32F4xx Flash Memory
 * @version 0.1
 * @date 2023-11-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include <stm32f4xx.h>

/** @defgroup FLASH_Program_Parallelism FLASH Program Parallelism
  * @{
  */
#define FLASH_PSIZE_BYTE           0x00000000U
#define FLASH_PSIZE_HALF_WORD      0x00000100U
#define FLASH_PSIZE_WORD           0x00000200U
#define FLASH_PSIZE_DOUBLE_WORD    0x00000300U
#define CR_PSIZE_MASK              0xFFFFFCFFU

/** @defgroup FLASH_Keys FLASH Keys
  * Taken from STM32 HAL
  */ 
#define RDP_KEY                  ((uint16_t)0x00A5)
#define FLASH_KEY1               0x45670123U
#define FLASH_KEY2               0xCDEF89ABU
#define FLASH_OPT_KEY1           0x08192A3BU
#define FLASH_OPT_KEY2           0x4C5D6E7FU

// User Flash Start defined in linker script
#define FLASH_USER_START 0x08060000

// Flash write successful
#define FLASH_WRITE_OK 0
// Invalid Flash Address given
#define FLASH_WRITE_ADDR_INVALID 1
// Failed to assert write permissions on the flash memory
#define FLASH_WRITE_NOACC 1
// Data Requested does not exist
#define FLASH_WRITE_NODAT 2


static void hal_FLASH_Lock(void){
  FLASH->CR |= FLASH_CR_LOCK;
}

// Unlock the flash memory
static inline uint8_t hal_FLASH_Unlock(void){
  // Check if already unlocked
  if(((FLASH->CR) & (FLASH_CR_LOCK)) != 0) {

    /* Authorize the FLASH Registers access */
    FLASH->KEYR = FLASH_KEY1; // Keys taken from STM32 HAL
    FLASH->KEYR = FLASH_KEY2;

    /* Verify Flash is unlocked */
        if((FLASH->CR & FLASH_CR_LOCK) != 0) {
            return FLASH_WRITE_NOACC;
        }
  }
  return FLASH_WRITE_OK;
}

/**
 * @brief Write a full word to flash memory (two half words)
 * 
 * @param Address Physical address to write to
 * @param data1 first half of word
 * @param data2 second half of word
 * @returns Flash Write Error
 */
static inline uint8_t hal_FLASH_WriteFW(uint32_t Address, uint32_t data1, uint32_t data2){
    // Check if write address is valid
    if(Address < FLASH_USER_START){
        return FLASH_WRITE_ADDR_INVALID;
    }

    // Unlock Flash in preparation for write
    if(hal_FLASH_Unlock() == FLASH_WRITE_NOACC) return FLASH_WRITE_NOACC;

    // Configure flash register to receive data
    ((FLASH->CR) &= ~(FLASH_CR_PSIZE));
    FLASH->CR |= FLASH_PSIZE_DOUBLE_WORD;
    FLASH->CR |= FLASH_CR_PG;

    // Write Data (first half)
    *(__IO uint32_t*)Address = data1;

  /* Barrier to ensure programming is performed in 2 steps, in right order
    (independently of compiler optimization behavior) */
    __ASM volatile ("isb 0xF":::"memory");

    // write data (second half)
    *(__IO uint32_t*)(Address+4) = data2;

    // Finish by locking the flash
    hal_FLASH_Lock();

    // Return OK
    return FLASH_WRITE_OK;
}

/**
 * @brief Write a single word to flash memory (uint32) 
 * 
 * @param Address The Hardware Address to write to
 * @param data The Data
 * @returns Flash Write Error
 */
static inline uint8_t hal_FLASH_WriteHW(uint32_t Address, uint32_t data){
    // Check if write address is valid
    if(Address < FLASH_USER_START){
        return FLASH_WRITE_ADDR_INVALID;
    }

    // Unlock Flash in preparation for write
    if(hal_FLASH_Unlock() == FLASH_WRITE_NOACC) return FLASH_WRITE_NOACC;
    
    // Configure flash register to receive data
    ((FLASH->CR) &= ~(FLASH_CR_PSIZE));
    FLASH->CR |= FLASH_PSIZE_WORD;
    FLASH->CR |= FLASH_CR_PG;

    // Write Data
    *(__IO uint32_t*)Address = data;

    // Finish by locking the flash
    hal_FLASH_Lock();

    // Return OK
    return FLASH_WRITE_OK;
}

/**
 * @brief 
 * 
 * @param Address 
 * @return const volatile* 
 */
#define hal_FLASH_Read(Address) ((const volatile uint32_t*)Address)

