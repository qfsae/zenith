/**
 * @brief I/O pin definitions for the steering wheel pcb
 * Note that there are mistakes in the silkscreen (ex. button 2 labelled as 4).
 * These definitions use the schematic net names as the source of truth.
 * Trace the net to a connector to find the right output.
 */

#ifndef STEERING_IO_H
#define STEERING_IO_H

#define STEERING_STATUS_LED PB14
#define STEERING_NEOPIXEL PB3
#define STEERING_TRANSLATOR_OE PB4
#define STEERING_CAN_OE PA10

#define STEERING_SPI1_NSS PA4
#define STEERING_SPI1_SCK PA5
#define STEERING_SPI1_MISO PA6
#define STEERING_SPI1_MOSI PA7

#define STEERING_BUTTON_1 PC6
#define STEERING_BUTTON_2 PC5
#define STEERING_BUTTON_3 PB15
#define STEERING_BUTTON_4 PC4



#endif
