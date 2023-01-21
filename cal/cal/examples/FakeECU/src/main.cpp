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

#define SPI_CAN 9

mcp2515_can can(SPI_CAN);
CAL::CAL cal;


void setup() {
  Serial.begin(9600);
  while(can.begin(CAN_500KBPS) != CAN_OK) {
    Serial.println("CAN Init Error..");
    delay(100);
  }
  Serial.println("CAN Bus Initialized");

  // Ensure Values Init to false
  cal.updateVar(CAL::DATA_DASH::UpShift, false);  //0 or false works
  cal.updateVar(CAL::DATA_DASH::DownShift, 0);
}

const int redLine = 9000;
bool upShift = false;
void send_wrapper(CAL::CAN_msg_t msg) {
  can.sendMsgBuf(msg.id, 0, msg.len, msg.data);
}

int rpm = 0;
int speed = 0;
void loop() {
  // Increase by odd number to add realism
  // int rpm = cal.returnVar(CAL::DATA_ECU::EngineRPM);
  cal.updateVar(CAL::DATA_ECU::EngineRPM, rpm);
  cal.updateVar(CAL::DATA_ECU::EngineTemp, 101);
  cal.updateVar(CAL::DATA_ECU::CoolantTemp, 50);
  cal.updateVar(CAL::DATA_ECU::VehicleSpeed, speed);

  send_wrapper(cal.package(CAL::DATA_ECU::EngineRPM));
  send_wrapper(cal.package(CAL::DATA_ECU::EngineTemp));
  send_wrapper(cal.package(CAL::DATA_ECU::CoolantTemp));
  send_wrapper(cal.package(CAL::DATA_ECU::VehicleSpeed));

  // Engine RPM Updates to Serial
  delay(100);
  if (rpm > redLine) {
    rpm = 0;
    speed = 0;
  } else {
    rpm += 147;
    speed += 2;
  }
}