#include <SPI.h>

#include "board_FT813.h"

// GPIO Pins
#include "ugfx-arduino-gdisp-FT813.h"
//#include "ugfx-arduino-ssd1351-pins.h"

#define GPIO_RESET ft813_pins.reset
#define GPIO_CS ft813_pins.cs

#define SPILOWSPEED 1000000
#define SPIHIGHSPEED 1000000
#define SPIBITORDER MSBFIRST
#define SPIMODE SPI_MODE0

static SPISettings spiSettings(SPILOWSPEED, SPIBITORDER, SPIMODE);

void FT813_init_board(void) {
  Serial.println("init board");
  pinMode(GPIO_RESET, OUTPUT);
  pinMode(GPIO_CS, OUTPUT);
}

void FT813_post_init_board(void) {
  // speed up SPI (12 MHz)
  Serial.println("post init board");
  spiSettings = SPISettings(SPIHIGHSPEED, SPIBITORDER, SPIMODE);
}

void FT813_setpin_reset(int state) {  // TODO implement PD_N reset pin
  if(state)
    digitalWrite(GPIO_RESET, 0);
  else
    digitalWrite(GPIO_RESET, 1);
}

void FT813_acquirebus(void) {
  SPI.beginTransaction(spiSettings);
  digitalWrite(GPIO_CS, LOW);
}

void FT813_releasebus(void) {
  digitalWrite(GPIO_CS, HIGH);
  SPI.endTransaction();
}

uint8_t FT813_write_data(uint8_t data) {
  uint8_t val = SPI.transfer(data);
  Serial.println(data, HEX);
  return val;
}

void debug_println(int num) {
  Serial.println(num);
}
