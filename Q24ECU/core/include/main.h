/**
 * @file main.h
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-11-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "interfaces/interface_uart.h"
#include "hal/hal_gpio.h"
#include "hal/hal_adc.h"
#include "hal/hal_uart.h"
#include "hal/hal_tim_basic.h"
#include "hal/hal_flash.h"
#include "taskHandlers.h"
#include "nvicConfig.h"
#include "interrupts.h"

extern void SystemInit(void);

extern int main(void);