#include <Arduino.h>
#include <cal.hpp>
#include "mcp2515_can.h"
#include <SPI.h>

mcp2515_can can(9);


void setup() {
  Serial.begin(9600);
  while(can.begin(CAN_500KBPS) != CAN_OK){
    Serial.println("CAN Init Error..");
    delay(100);
  }
  Serial.println("CAN Bus Initialized");
}

int gear = 1;
int engineRPM = 0;

void loop() {
  if(can.checkReceive() == CAN_MSGAVAIL){
    CAL::CAN_msg_t can_recv;
    can.readMsgBuf(&can_recv.len, can_recv.data);
    can_recv.id = can.getCanId();
    CAL::update(can_recv, CAL::DATA_DASH::Gear, &gear);
    CAL::update(can_recv, CAL::DATA_ECU::EngineRPM, &engineRPM);
  }
  Serial.println(String("Gear: ") + gear + String("\tEngine RPM: ") + engineRPM);
  //delay(100);
}