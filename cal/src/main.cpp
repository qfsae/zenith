#include <Arduino.h>
#include "cal.hpp"
#include <mcp2515_can.h>
#include <SPI.h>

// 0 Disabled / 1 ID Debug / 2 ecu throt 
#define debug 2

// THIS IS JUST FOR TESTING
/**
 * The actual libraries are under cal.hpp and cal_ecu.cpp
 * this file is an example/test implementation of the abstraction layer
 * 
 */

mcp2515_can can(9);

void setup() {
  Serial.begin(9600); // 115200
  while(can.begin(CAN_1000KBPS) != CAN_OK){
    Serial.println("Can Init Error\nTrying Again..\n");
    delay(100);
  }
  Serial.println("Can Bus Initialized\n");
}

int updatertn = 0;
int derta;

void loop() {

  // Check for CAN message recieved
  if(CAN_MSGAVAIL == can.checkReceive()){

    // CAN message container
    CAN_msg_t can_recv;

    // Read CAN message using whatever hardware logic
    can.readMsgBuf(&can_recv.len, can_recv.data);
    can_recv.id = can.getCanId();

    // Run applicable update methods (only ecu for now)
    updatertn = CAL::update(can_recv, CAL::DATA_ECU::ThrottlePosition, &derta);
  }

  if(updatertn == 0){
    Serial.print(updatertn);
    Serial.print("\t");
    Serial.print(derta);
    Serial.print("\n");
  }
  
  

}