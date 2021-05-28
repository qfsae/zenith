/**
 * @brief I/O pin definitions for the steering wheel pcb
 * Note that there are mistakes in the silkscreen (ex. button 2 labelled as 4).
 * These definitions use the schematic net names as the source of truth.
 * Trace the net to a connector to find the right output.
 */

#ifndef STEERING_IO_H
#define STEERING_IO_H

#define STEERING_STATUS_LED 29
#define STEERING_NEOPIXEL 19
#define STEERING_TRANSLATOR_OE 20
#define STEERING_CAN_OE 10

#define STEERING_SPI1_NSS A4
#define STEERING_SPI1_SCK A5
#define STEERING_SPI1_MISO A6
#define STEERING_SPI1_MOSI A7

#endif
