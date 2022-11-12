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

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];
    if (CAN_MSGAVAIL == can.checkReceive()) {         // check if data coming
        can.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = can.getCanId();
        if (canId == 0x118) {
            uint8_t tps = buf[1];
            Serial.print("tps = ");
            Serial.println(tps);
        }
        if (canId == 0x119) {
            //Serial.println("second ECU message found");
        }
    }
}