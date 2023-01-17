/**
 * @file main.cpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Mock ECU example program using Arduino
 * @version 2.3
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <SPI.h>
#include "mcp2515_can.h"
#include <cal.hpp>

mcp2515_can can(9);
CAL::CAL cal;


void setup() {
  Serial.begin(9600);
  while(can.begin(CAN_500KBPS) != CAN_OK){
    Serial.println("CAN Init Error..");
    delay(100);
  }
  Serial.println("CAN Bus Initialized");

  // Ensure Values Init to false
  cal.updateVar(CAL::DATA_DASH::UpShift, false);  //0 or false works
  cal.updateVar(CAL::DATA_DASH::DownShift, 0);
}

const int redLine = 12000;
bool upShift = false;

void loop() {

  // Initiate Shift Check when engine at redline
  if(cal.returnVar(CAL::DATA_ECU::EngineRPM) > redLine){

    // Check for incoming CAN messages
    if(can.checkReceive() == CAN_MSGAVAIL){
      CAL::CAN_msg_t can_recv;
      can.readMsgBuf(&can_recv.len, can_recv.data);
      can_recv.id = can.getCanId();
      cal.updatePackage(can_recv);
    }

    // Check if Upshift was made
    if(upShift){
      // Lower Engine RPM
      cal.updateVar(CAL::DATA_ECU::EngineRPM, 3500);
      // Reset Upshift Trigger
      upShift = false;
    }
  }
  // While Engine not at redline, increase engine RPM
  else{

    // Increase by odd number to add realism
    cal.updateVar(CAL::DATA_ECU::EngineRPM, (cal.returnVar(CAL::DATA_ECU::EngineRPM) + 147));

    // Engine RPM Updates to CAN
    CAL::CAN_msg_t &msg = cal.package(CAL::DATA_ECU::EngineRPM);
    can.sendMsgBuf(msg.id, 0, msg.len, msg.data);

    // Engine RPM Updates to Serial
    Serial.println(String("RPM: ") + cal.returnVar(CAL::DATA_ECU::EngineRPM));
    delay(100);
  };  

}