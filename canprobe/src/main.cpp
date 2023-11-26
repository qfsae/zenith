#include <Arduino.h>
#include <SPI.h>
#include "mcp2515_can.h"

#define SPI_CS_PIN 9
#define CAN_ID 0x280
mcp2515_can can(SPI_CS_PIN);


void setup() {
  Serial.begin(115200);
  while(CAN_OK != can.begin(CAN_1000KBPS)) {
    Serial.println("CAN BUS init fail, retrying...");
    delay(1000);
  }
  Serial.println("CAN BUS init ok!");
}

void loop() {
  unsigned char len = 6;
  unsigned char buf[len];
  if (CAN_MSGAVAIL == can.checkReceive()) {
    can.readMsgBuf(&len, buf);

    unsigned long canId = can.getCanId();
    // filter by canId
    if(canId == CAN_ID){
      //byte 0: motor rpm
      uint8_t rpm = buf[0] * 100;
      Serial.print("Motor RPM: ");
      Serial.println(rpm);

      //byte 1: drive status
      uint8_t drive_status = buf[1];
      if(drive_status &  0x00){
        Serial.println("TS Active");
      } 
      if(drive_status &  0x01){
        Serial.println("BMS Fault");
      } 
      if(drive_status &  0x02){
        Serial.println("Temp Fault");
      } 
      if(drive_status &  0x04){
        Serial.println("Inverter Fault");
      } 
      if(drive_status &  0x08){
        Serial.println("VCU Fault");
      }  
      if(drive_status &  0x10){
        Serial.println("Throttle Fault");
      } 
      if(drive_status &  0x20){
        Serial.println("Brake Fault");
      }
      
      //byte 2: vbatt voltage
      uint8_t vbatt_voltage = buf[2] / 0.1421;
      Serial.print("VBatt Voltage (V): ");
      Serial.println(vbatt_voltage);

      //byte 3-4: motor power
      uint16_t motor_power = (buf[4] << 8) | (buf[3] & 0xFF);
      Serial.print("Motor Power (W): ");
      Serial.println(motor_power);

      //byte 5: coolant temp
      uint8_t coolant_temp = buf[5];
      Serial.print("Coolant Temp (C): ");
      Serial.println(coolant_temp);
    }
   
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}