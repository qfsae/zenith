#include <Arduino.h>
#include "cal.hpp"
#include <mcp2515_can.h>
#include <SPI.h>

// THIS IS JUST FOR TESTING
/**
 * The actual libraries are under cal.hpp and cal_ecu.cpp
 * this file is an example/test implementation of the abstraction layer
 * 
 */

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

    ecu.update(can_recv);

    Serial.print("Transmisson Recieved from ID: ");
    Serial.print(can_recv.id, HEX);
    Serial.print("\n");
  }
}