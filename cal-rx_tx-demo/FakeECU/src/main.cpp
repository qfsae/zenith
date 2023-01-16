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
  cal.updateVar(CAL::DATA_DASH::Gear, 2);
}

int rpm = 3500;
int redLine = 12000;


void shiftUp(){
  cal.updateVar(CAL::DATA_DASH::Gear, cal.returnVar(CAL::DATA_DASH::Gear)+1);
}

void shiftDown(){
  cal.updateVar(CAL::DATA_DASH::Gear, cal.returnVar(CAL::DATA_DASH::Gear)-1);
}

void loop() {
  rpm+=100;
  if(rpm > redLine){
    rpm = 3500;
    shiftUp();
    CAL::CAN_msg_t &mg = cal.package(CAL::DATA_DASH::Gear);
    can.sendMsgBuf(mg.id, 0, mg.len, mg.data);
  }
  if(cal.returnVar(CAL::DATA_DASH::Gear) > 7){
    cal.updateVar(CAL::DATA_DASH::Gear, 1);
  }
  cal.updateVar(CAL::DATA_ECU::EngineRPM, rpm);
  CAL::CAN_msg_t &msg = cal.package(CAL::DATA_ECU::EngineRPM);
  can.sendMsgBuf(msg.id, 0, msg.len, msg.data);
  Serial.println(String("RPM: ") + cal.returnVar(CAL::DATA_ECU::EngineRPM));
  delay(100);
}