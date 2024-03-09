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


//can related
unsigned char len = 0;
unsigned char buf[8];
unsigned int canID;
unsigned int id = 100;

//bools
unsigned char lockout = 1;
unsigned char enabled = 0;
unsigned char discharge = 0;
//1 for forward, 0 for reverse
unsigned char direction = 1;
//1 for speed mode, 0 for torque mode
unsigned char commandMode = 0;


//torque mode
uint16_t commanded_torque = 0;
uint16_t speed_limit = 0;
//0 for default limits, limit otherwise
uint16_t torque_limit = 0;

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
    if(CAN_MSGAVAIL == CAN.checkReceive())    //check if data is coming
    {
        CAN.readMsgBuf(&len, buf);    //read data,  len: data length, buf: data buffer
        canID = CAN.getCanId();       //getting the ID of the incoming message

        Serial.print("ID is: ");
        Serial.print(canID, HEX);     //printing the ID in its standard form, HEX

        if (canID == id) {
            Serial.print("Message Length: ");
            Serial.println(len);
            
            //disable lockout under disable command
            if (buf[5] == 0) {
                lockout = 0;
            }

            if (!lockout) {
                enabled = buf[5] & 0x01;
                discharge = buf[5] & (0x01 << 1);
                commandMode = buf[5] & (0x01 << 2);

                torque_limit = buf[6] << 8 | buf[7];

                if (enabled) {
                    //torque mode
                    if (commandMode == 0) {
                        commanded_torque = buf[0] << 8 | buf[1];
                        speed_limit = buf[2] << 8 | buf[3];
                    }
                    //speed mode
                    else {
                        torque_override = buf[0] << 8 | buf[1];
                        commanded_speed = buf[2] << 8 | buf[3];
                    }
                }
                else {
                    //direction can only be set when disabled.
                    direction = buf[4] & 0x01;
                }
            }

            for(int i = 0; i<len; i++)    //looping on the incoming data to print them
            {
                Serial.write(buf[i]);     //Serial.write prints the character itself
            }
            Serial.println("Enabled: " + enabled);
            Serial.println("Torque: " + commanded_torque);
            Serial.println("Direction: " + direction);
            Serial.println("\n\t*****************\n");
        }
        
    }
}