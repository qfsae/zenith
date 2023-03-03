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
}

uint32_t id = 0x120;
unsigned char slen = 2;
unsigned char sbuf[8] = {0, 1};


void loop() {
    unsigned char rlen = 0;
    unsigned char rbuf[8] = {0, 1};

    if (CAN_MSGAVAIL == can.checkReceive()) {         // check if data coming
        Serial.println("New Message from ID: " + can.getCanId());
        can.readMsgBuf(&rlen, rbuf);    // read data,  len: data length, buf: data buf
        for (int i = 0; i < rlen; i++)
        {
          Serial.print(rbuf[i] + "\t");
        }
        Serial.println();
        
    }
    else{
      can.sendMsgBuf(id, 0, slen, sbuf);
      Serial.println("Sent Message on ID: " + id);
    }
}