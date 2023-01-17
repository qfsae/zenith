#include <Arduino.h>
#include <SPI.h>
#include "mcp2515_can.h"
#include "encoder.hpp"

mcp2515_can can(9);
CAL::Encoder cal;


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

void loop() {
  // Shifting (When RPM at Redline)
  if(cal.returnVar(CAL::DATA_ECU::EngineRPM) > redLine){
    // Fake Lower RPM
    cal.updateVar(CAL::DATA_ECU::EngineRPM, 3500);

    // Send Upshift
    cal.updateVar(CAL::DATA_DASH::UpShift, 1);
    CAL::CAN_msg_t &mg = cal.package(CAL::CAN_ID::DASH);
    can.sendMsgBuf(mg.id, 0, mg.len, mg.data);

    // Reset UpShift Trigger !!
    cal.updateVar(CAL::DATA_DASH::UpShift, 0);
  }
  
  // Engine RPM Updates to CAN
  cal.updateVar(CAL::DATA_ECU::EngineRPM, (cal.returnVar(CAL::DATA_ECU::EngineRPM) + 100));
  CAL::CAN_msg_t &msg = cal.package(CAL::DATA_ECU::EngineRPM);
  can.sendMsgBuf(msg.id, 0, msg.len, msg.data);

  // Engine RPM Updates to Serial
  Serial.println(String("RPM: ") + cal.returnVar(CAL::DATA_ECU::EngineRPM));
  delay(100);
}