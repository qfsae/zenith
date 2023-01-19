/**
 * @file main.cpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief CAL Steering Demo Using Arduino
 * @version 2.0
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <cal.hpp>
#include "mcp2515_can.h"
#include <SPI.h>

mcp2515_can can(9);

CAL::CAL cal;

void setup() {
  Serial.begin(115200);
  while(can.begin(CAN_500KBPS) != CAN_OK){
    Serial.println("CAN Init Error..");
    delay(100);
  }
  Serial.println("CAN Bus Initialized");
}

int engineRPM = 0;

void loop() {
  // Check for new messages on CAN bus
  if(can.checkReceive() == CAN_MSGAVAIL){
    CAL::CAN_msg_t can_recv;
    can.readMsgBuf(&can_recv.len, can_recv.data);
    can_recv.id = can.getCanId();
    cal.updatePackage(can_recv);
/*
    // Update Packages with incoming data
    cal.updatePackage(can_recv);
    for (int i = 0; i < can_recv.len; i++)
    {
      Serial.print(can_recv.data[i] + String("\t"));
    }
    Serial.print("\n");
    */
  }

  // Method 1 of returning data - by reference (works with all data types)
  cal.returnVar(CAL::DATA_ECU::EngineRPM, engineRPM);
  // Print RPM to Serial Terminal
  Serial.print(String("Engine RPM: ") + engineRPM);

  // Method 2 of returning data - return (ONLY WORKS WITH INTEGERS!!)
  if(cal.returnVar(CAL::DATA_ECU::EngineRPM) >= 12000){
    // simulate driver sending shift message
    cal.updateVar(CAL::DATA_DASH::UpShift, true);
    CAL::CAN_msg_t &can_send = cal.package(CAL::CAN_ID::DASH);
    can.sendMsgBuf(can_send.id, 0, can_send.len, can_send.data);
    Serial.print("\tUpshift\n");
    cal.updateVar(CAL::DATA_DASH::UpShift, false);
  }
  else{
    Serial.print("\n");
  }
  delay(20);
}