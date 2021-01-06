#include <Arduino.h>
//#include <ugfx-config-ssd1351.h>
#include <SPI.h>
#define MOSI_PIN 11
#define MISO_PIN 12
#define SPI_CLK_PIN 13

byte byte1,byte2,byte3,byte4;

void setup()
{
  // Initialize the Serial bus at 115.2 kBaud
  Serial.begin(115200);
  pinMode(10, OUTPUT);

  SPI.begin(); 

  // ugfx sample was here ... 
}

void loop()
{  
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);
  
  //SPI.transfer(0x00);
  //SPI.transfer(0x00);


  SPI.transfer(0x0C);  // 00001100 memory read on address 0xC0000
  SPI.transfer(0x00); // 00000000
  SPI.transfer(0x00); // 00000000
  SPI.transfer(0x00); // 00000000 dummy byte
  byte1 = SPI.transfer(0x00);
  byte2 = SPI.transfer(0x00);
  byte3 = SPI.transfer(0x00);
  byte4 = SPI.transfer(0x00);
  
  digitalWrite(10, HIGH);
  SPI.endTransaction();

  Serial.println(byte1, HEX);
  Serial.println(byte2, HEX);
  Serial.println(byte3, HEX);
  Serial.println(byte4, HEX);
    // digitalWrite(13, HIGH);
    delay(500);
    // digitalWrite(13, LOW);
    // delay(500);
};