#include <Arduino.h>
#include <SPI.h>
#include "mcp2515_can.h"

#define SPI_CS_PIN 9

mcp2515_can can(SPI_CS_PIN);

void setup() {
    Serial.begin(115200);
    while (CAN_OK != can.begin(CAN_1000KBPS)) {
        Serial.println("CAN INIT FAIL");
        Serial.println("TRY AGAIN");
        delay(1000);
  }
  Serial.println("Initialization Success");
  pinMode(A0, INPUT);
}

uint32_t id = 0x0500;
uint32_t max_id = 0x07f0;
/**
 * 0x0F4 / 0x0FC for 50hz
 * 0x0F0 / 0xf8 for 200hz
 * 
 */


void loop() {

  unsigned char slen = 8;
  unsigned int volt = analogRead(A0)*5.00*1000/1024;
  unsigned char a = ((int)(volt) >> 8);
  unsigned char b = ((int)(volt));
  unsigned char sbuf[8] = {a, b, a, b, a, b, a, b};
  unsigned char rlen = 0;

    //unsigned char rbuf[8] = {((uint8_t)(513) >> 8), 513};
/*
    if (CAN_MSGAVAIL == can.checkReceive()) {         // check if data coming
        can.readMsgBuf(&rlen, rbuf);    // read data,  len: data length, buf: data buf
        Serial.println("New Message from ID: " + String(can.getCanId()));
        
        for (int i = 0; i < rlen; i++)
        {
          Serial.print(String(rbuf[i]) + "\t");
        }
        Serial.println();
        
    }
    else{*/
      
      can.sendMsgBuf(id, 0, slen, sbuf);
      //Serial.println("Sent Message on ID: " + String(id));
    //}
}