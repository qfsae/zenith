#include <Arduino.h>
#include "cal.hpp"
#include <mcp2515_can.h>
#include <SPI.h>

CAL::ECU ecu;
mcp2515_can can(9);

void setup() {
  Serial.begin(9600);
  while(can.begin(CAN_500KBPS) != CAN_OK){
    Serial.println("Can Init Error\nTrying Again..");
    delay(100);
  }
  Serial.println("Can Bus Initialized");
}

void loop() {

  if(CAN_MSGAVAIL == can.checkReceive()){

  CAN_msg_t can_recv;
  can.readMsgBuf(&can_recv.len, can_recv.data);
  can_recv.id = can.getCanId();

  switch (can_recv.id)
  {
    case CAL::MOTEC_ID::ECU_1:
      ecu.update(can_recv);
      break;
    case CAL::MOTEC_ID::ECU_2:
      ecu.update(can_recv);
  default:
    Serial.print("Transmisson Recieved from Unknown ID: ");
    Serial.print(can_recv.id, HEX);
    Serial.print("\n");
    break;
  }
  }
}