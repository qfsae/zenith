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
  Serial.println("Initialization Success\n Running VCU CAN Test...");
}

unsigned char bufO[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

void loop() {
    unsigned char len = 0;
    unsigned char buf[8];
    if (CAN_MSGAVAIL == can.checkReceive()) {
        uint32_t id = can.getCanId();
        Serial.print("ID: ");
        Serial.println(id);
        can.readMsgBuf(&len, buf);
        
    }else{
        can.sendMsgBuf(200, 0, 8, bufO);
    }
}