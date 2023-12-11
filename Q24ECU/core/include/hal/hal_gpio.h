#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <sys/stat.h>

#include <stm32f446xx.h>
#include "hal_clock.h"
#include "pins.h"

// CPU Frequency
//#define FREQ 180000000//SystemCoreClock//16000000

#define GPIO(bank) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400U * (bank)))
enum GPIO_MODE_IO   { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };
enum GPIO_PULL_MODE { GPIO_RESET, GPIO_PULLUP, GPIO_PULLDOWN};



/**
 * @brief Set the PinMode of a GPIO Pin
 * 
 * @param pin PIN(bank, number)
 * @param mode GPIO_MODE::<INPUT, OUTPUT, AF, ANALOG>
 */
static inline void gpio_set_mode(uint16_t pin, enum GPIO_MODE_IO mode) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    RCC->AHB1ENR |= BIT(PINBANK(pin));       // Enable GPIO clock
    gpio->MODER &= ~(3U << (n*2));
    gpio->MODER |= (mode & 3U) << (n * 2);   // Set new mode

}

/**
 * @brief Set GPIO Alternate Function
 * 
 * @param pin 
 * @param af_num 
 */
static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}

/**
 * @brief Write a Digital Value to a pin
 * 
 * @param pin PIN(bank, number)
 * @param value <true, false>
 */
static inline void gpio_write(uint16_t pin, bool value) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (value ? 0 : 16);
}

static inline void gpio_toggle_pin(uint16_t pin){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1UL << PINNO(pin)) << (gpio->ODR & (1UL << PINNO(pin)) ? 16 : 0);
}

static inline bool gpio_read_idr(uint16_t pin) {
    const GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    return (gpio->IDR & (1U << PINNO(pin)));
}

static inline bool gpio_read_odr(uint16_t pin){
    const GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    return (gpio->ODR & (1U << PINNO(pin)));
}

static inline void gpio_pull(uint16_t pin, enum GPIO_PULL_MODE mode){
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->PUPDR &= ~(3U << (PINNO(pin)*2));
    if(mode!=GPIO_RESET) gpio->PUPDR |= mode << (2*(PINNO(pin)));
}

// t: expiration time, prd: period, now: current time. Return true if expired
static inline bool timer_expired(volatile uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // First poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}
