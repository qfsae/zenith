#ifndef UGFX_ARDUINO_FT813_H
#define UGFX_ARDUINO_FT813_H

#include <stdint.h>

typedef struct {
  const uint8_t reset;
  const uint8_t cs;
} ft813_pins_t;

extern const ft813_pins_t ft813_pins;

#endif // UGFX_ARDUINO_FT813_H
