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

CAL::ECU ecu;
mcp2515_can can(9);

void setup() {
  Serial.begin(9600);
  while(can.begin(CAN_500KBPS) != CAN_OK){
    Serial.println("Can Init Error\nTrying Again..\n");
    delay(100);
  }
  Serial.println("Can Bus Initialized\n");
}

void loop() {

  // Check for CAN message recieved
  if(CAN_MSGAVAIL == can.checkReceive()){

    // CAN message container
    CAN_msg_t can_recv;

    // Read CAN message using whatever hardware logic
    can.readMsgBuf(&can_recv.len, can_recv.data);
    can_recv.id = can.getCanId();

    // Run applicable update methods (only ecu for now)
    ecu.update(can_recv);

    // Debug
    if(debug == 1){
      Serial.print("Transmisson Recieved from ID: ");
      Serial.print(can_recv.id, HEX);
      Serial.print("\n");
    }
    if(debug == 2){
      Serial.print("Throttle Position: ");
      Serial.print(ecu.tPos);
      Serial.print("\n");
    }
  }
}