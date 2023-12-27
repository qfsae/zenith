/**
 * @file testTasks.c
 * @author Jacob Chisholm (https://jchisholm204.github.io/)
 * @brief Test Task file - Collection of test tasks
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "taskHandlers.h"
#include "main.h"
#include "task.h"
#include "hal/hal_gpio.h"
#include <stdio.h>

long unsigned int counter = 0;

typedef const struct {
    uint8_t TS2;
    uint8_t TS1;
    uint8_t BRP;
} hal_CAN_bit_timing_config_t;

hal_CAN_bit_timing_config_t can_configs[6] = {{2, 12, 60}, {2, 12, 30}, {2, 12, 24}, {2, 12, 12}, {2, 12, 6}, {1, 7, 5}};

typedef enum {CAN_50KBPS, CAN_100KBPS, CAN_125KBPS, CAN_250KBPS, CAN_500KBPS, CAN_1000KBPS} hal_CAN_BITRATE;

void hal_CANSetFilter(uint8_t index, uint8_t scale, uint8_t mode, uint8_t fifo, uint32_t bank1, uint32_t bank2) {
    if(index > 27) return;

    // Deactivate filter
    CAN1->FA1R &= ~(0x1UL << index);

    if(scale == 0) {
        CAN1->FS1R &= ~(0x1UL << index); // Set filter to Dual 16-bit scale configuration
    }
    else {
        CAN1->FS1R |= (0x1UL << index); // Set filter to single 32-bit configuration
    }
    if(mode == 0) {
        CAN1->FM1R &= ~(0x1UL << index); // Set filter to Mask Mode
    }
    else {
        CAN1->FM1R |= (0x1UL << index); // Set filter to List Mode
    }

    if(fifo == 0) {
        CAN1->FFA1R &= ~(0x1UL << index); // Set filter assigned to FIFO 0
    }
    else {
        CAN1->FFA1R |= (0x1UL << index); // Set filter assigned to FIFO 1
    }

    CAN1->sFilterRegister[index].FR1 = bank1;
    CAN1->sFilterRegister[index].FR2 = bank2;

    CAN1->FA1R |= (0x1UL << index); // Activate Filter
}

void tsk_Test1(void *param){
    printf("Starting CAN Init..\n");
    (void)(param); // Cast unused variable to void
    // Enable CAN1 Clock
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    // Enable CAN1 gpio pins on MockECU
    printf("Setting Pins..\n");
    gpio_set_mode(PIN('A', 11), GPIO_MODE_AF);
    gpio_set_mode(PIN('A', 12), GPIO_MODE_AF);
    gpio_set_af(PIN('A', 11), 9);
    gpio_set_af(PIN('A', 12), 9);
    printf("Pins Set\n");
    printf("Entering CAN init mode\n");
    
    // Set up the CAN1 Periphrial
    CAN1->MCR |= 0x1UL; // Enter into initialization mode
    while(!(CAN1->MSR & 0x1UL)); // Wait to enter into initialization mode
    CAN1->MCR = 0x51UL; // Setup: autotransmission disable (enable = 0x41UL)
    // Setup: bit rates
    CAN1->BTR &= ~(((0x03) << 24 ) | ((0x07) << 20) | ((0x0F) << 16) | (0x1FFUL));
    CAN1->BTR |= (((can_configs[CAN_1000KBPS].TS2 - 1) & 0x07) << 20) | (((can_configs[CAN_1000KBPS].TS1 - 1) & 0x0F) << 16) | ((can_configs[CAN_1000KBPS].BRP - 1) & 0x1FF);
    // Configure Filters to default values
    CAN1->FMR |= 0x1UL; // Filters enter initalization mode
    CAN1->FMR &= 0xFFFC0FF; // Clear start bank
    printf("Setting Filters..\n");
    hal_CANSetFilter(0, 1, 0, 0, 0x0UL, 0x0UL);
    hal_CANSetFilter(14, 1, 0, 0, 0x0UL, 0x0UL);

    CAN1->FMR &= ~(0x1UL); // Deactivate Initialization Mode

    printf("Entering Normal Mode");
    uint16_t TimeoutMS = 1000;
    bool goodInit = false;
    CAN1->MCR &= ~(0x1UL);
    for(uint16_t wait_ack = 0; wait_ack < TimeoutMS; wait_ack++) {
        if((CAN1->MSR & 0x1UL) == 0) {
            goodInit = true;
            break;
        }
        printf("CAN Init Failure.. Trying Again..\n");
        vTaskDelay(10);
    }
    if(goodInit == false){
        printf("CAN Init Failure: Stopping Task\n");
        vTaskDelay(portMAX_DELAY);
    }
    else{
        printf("CAN Init Success\n");
    }

    for (;;)
    {
        if(CAN1->RF0R & 0x3UL){
            printf("CAN MSG Recieved\n");
        }
        vTaskDelay(10);
        // Print out the systemtick timer once a second
        // printf("%ld\n", (long)xTaskGetTickCount());
        // vTaskDelay(1000);
    }
}


void tsk_BlinkLED(void *param){
    (void)param;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for(;;){
        // This task is currently not used so perma suspend it
        lastWakeTime = xTaskGetTickCount();
        counter++;
        vTaskDelayUntil(&lastWakeTime, 10);
    }
}

