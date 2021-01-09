#include <Arduino.h>
#include <ugfx-config-FT813.h>
#include <SPI.h>

//#include "QFSAE_SPI.h"

using namespace std;

#define MOSI_PIN 11
#define MISO_PIN 12
#define SPI_CLK_PIN 13

#define BYTE 4

int status = 0;
uint8_t test8 = 0x00;
uint16_t test = 0x0000;
uint16_t test2 = 0x0000;
uint32_t code = 0x00000000;
uint8_t id = 0x00;
uint8_t rx[BYTE];

void setup()
{
  // Initialize the Serial bus at 115.2 kBaud
  Serial.begin(115200);

  SPI.begin(); 
  // ugfx sample was here ...

  delay(3000);
  gfxInit();  


}

void loop()
{  
};