#include <Arduino.h>
//#include <ugfx-config-ssd1351.h>
#include <SPI.h>

#include "QFSAE_SPI.h"

using namespace std;

#define MOSI_PIN 11
#define MISO_PIN 12
#define SPI_CLK_PIN 13

#define BYTE 4

QSPI disp(10); 

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
  pinMode(10, OUTPUT);

  SPI.begin(); 
  disp = QSPI(10);
  // ugfx sample was here ...

  // SPI.transfer(0x0C);  // 00001100 memory read on address 0xC0000
  // SPI.transfer(0x00); // 00000000
  // SPI.transfer(0x00); // 00000000
  // SPI.transfer(0x00); // 00000000 dummy byte
  // byte1 = SPI.transfer(0x00);
  // byte2 = SPI.transfer(0x00);
  // byte3 = SPI.transfer(0x00);
  // byte4 = SPI.transfer(0x00);

  // test = QSPI::rd16(REG_HCYCLE);
  // QSPI::wr16(REG_HCYCLE, 0x225);
  
  // SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  // digitalWrite(10, LOW);
  // test = QSPI::rd16(REG_HCYCLE);
  // digitalWrite(10, HIGH);
  // SPI.endTransaction(); 
    delay(1000);
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    disp.sel();         // manually pull CS pin low
    SPI.transfer(0x00);
    SPI.transfer(0x00);
    SPI.transfer(0x00); // set active
    disp.desel();

    /* Configure display registers */
    disp.wr16(REG_HCYCLE, 548);
    disp.wr16(REG_HOFFSET, 43);
    disp.wr16(REG_HSYNC0, 0); 
    disp.wr16(REG_HSYNC1, 41); 
    disp.wr16(REG_VCYCLE, 292); 
    disp.wr16(REG_VOFFSET, 12); 
    disp.wr16(REG_VSYNC0, 0); 
    disp.wr16(REG_VSYNC1, 10); 
    disp.wr8(REG_SWIZZLE, 0); 
    disp.wr16(REG_PCLK_POL, 1); 
    disp.wr8(REG_CSPREAD, 1); 
    disp.wr16(REG_HSIZE, 480); 
    disp.wr16(REG_VSIZE, 272);

    /* Write first display list */
    disp.wr32(RAM_DL+0, 0x02000000); // CLEAR_COLOR_RGB(0,0,0) command
    disp.wr32(RAM_DL+4, 0x26000007); // CLEAR(1,1,1) command
    disp.wr32(RAM_DL+8, 0x1F000001); // BEGIN(BITMAPS);
    disp.wr32(RAM_DL+12, 0x9B86EFC6);  // VERTEX2II(220,110,31,'F'));
    disp.wr32(RAM_DL+16, 0x21000000);  // END()           
    disp.wr32(RAM_DL+20, 0x00000000); // DISPLAY();

    /* Display List Swap */
    disp.wr8(REG_DLSWAP, 0xFFFFFF01);     // render new screen when ready
    
    /* Enable backlight control */
    uint8_t gpio_dir = disp.rd8(REG_GPIO_DIR);
    disp.wr8(REG_GPIO_DIR, (0x80 | gpio_dir));
    uint8_t gpio = disp.rd8(REG_GPIO);
    disp.wr8(REG_GPIO, (0x080 | gpio));

    /* Divide main clock for PCLK; start video */
    disp.wr8(REG_PCLK, 5);

    SPI.endTransaction();

}

void loop()
{  
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    disp.wr32(RAM_DL+0, 0x02000000); // CLEAR_COLOR_RGB(0,0,0) command
    disp.wr32(RAM_DL+4, 0x26000007); // CLEAR(1,1,1) command
    disp.wr32(RAM_DL+8, 0x1F000001); // BEGIN(BITMAPS);
    disp.wr32(RAM_DL+12, 0x9B86EFC6);  // VERTEX2II(220,110,31,'F'));
    disp.wr32(RAM_DL+16, 0x21000000);  // END()
    disp.wr32(RAM_DL+20, 0x00000000); // DISPLAY();

    /* Display List Swap */
    disp.wr8(REG_DLSWAP, 0xFFFFFF01);     // render new screen when ready
    SPI.endTransaction();
    delay(500);
};