#include <SPI.h>          //SPI is used to talk to the CAN Controller
#include <mcp_can.h>
#include <can-serial.h>

// #define CAN_2518FD
#define CAN_2515

#ifdef CAN_2518FD
  #include "mcp2518fd_can.h"
  const int SPI_CS_PIN = 10;
  const int CAN_INT_PIN = 2;
  mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
  #include "mcp2515_can.h"
  const int SPI_CS_PIN = 9;
  const int CAN_INT_PIN = 2;
  mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

//delay can range from 3 - 500 ms
#define MESSAGE_DELAY 5


//can related
unsigned char len = 8;
unsigned char buf[8];
//should be set to controller id.
unsigned int id = 100;

//bools
unsigned char enabled = 0;
unsigned char discharge = 0;
//1 for forward, 0 for reverse
unsigned char direction = 1;
//1 for speed mode, 0 for torque mode
unsigned char commandMode = 0;
//0 for default limits, limit otherwise
uint16_t torque_limit = 0;


//torque mode
uint16_t commanded_torque = 0;
uint16_t speed_limit = 0;

//speed mode
uint16_t commanded_speed = 0;
uint16_t torque_override = 0;

void setup()
{
  Serial.begin(115200);   //to communicate with Serial monitor
  
//tries to initialize, if failed --> it will loop here for ever
START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))      //setting CAN baud rate to 500Kbps
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}

void loop()
{
    buf[5] = enabled | discharge << 1 | commandMode << 2;
    buf[4] = direction;
    buf[6] = torque_limit;

    if (commandMode == 0) {
        buf[0] = commanded_torque;
        buf[2] = speed_limit;
    }
    else {
        buf[0] = torque_override;
        buf[2] = commanded_speed;
    }
    //CAN.sendMsgBuf(msg ID, extended?, #of data bytes, data array);
    CAN.sendMsgBuf(id, 0, len, buf);
    delay(MESSAGE_DELAY);
}