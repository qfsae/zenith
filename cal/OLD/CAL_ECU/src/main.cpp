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
  Serial.begin(9600); // 115200
  while(can.begin(CAN_1000KBPS) != CAN_OK){
    Serial.println("Can Init Error\nTrying Again..\n");
    delay(100);
  }
  Serial.println("Can Bus Initialized\n");
}

bool last = true;

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
      //Serial.print("Transmisson Recieved from ID: ");
      //Serial.print(can_recv.id, HEX);
      //Serial.print("\t");
      //Serial.print("Throttle Position: ");
      //Serial.print("\n");
    /**
    if(can_recv.id == CAL::MOTEC_ID::ECU_2){
      Serial.print(can_recv.data[4], BIN);
      Serial.print("\t");
      Serial.print(can_recv.data[5], BIN);
      Serial.print("\n");
    }*/
  }
  
  Serial.print(ecu.cTemp);
  Serial.print("\t");
  Serial.print(ecu.dTemp);  
  Serial.print("\t");
  Serial.print(ecu.eTemp);
  Serial.print("\t");
  Serial.print(ecu.fTemp);
  Serial.print("\t");
  Serial.print(ecu.tTemp);
  Serial.print("\n");
  //Serial.println(ecu.tPos);
  
  
  
  //delay(100);
}