#include <SPI.h>

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif                           


//START OF CUSTOM CODE:
typedef struct{
    unsigned long address;
    unsigned short channel;
} IDTuple;

typedef struct {
    unsigned char battVolt
    unsigned char fPCurr
    unsigned char fPVolt
    unsigned char fpStatusField
    unsigned char kSState
    unsigned char tBCurr
    unsigned char tBVolt
    unsigned char tBStatusFault
    unsigned char tBStatusField
    unsigned char cF1Status
    unsigned char cF2Status
    unsigned char fICurr
    unsigned char fIVolt
    unsigned char fIStatusField
    unsigned char sRStatus
} PDM;

typedef struct {
    unsigned char rPM;
    unsigned char tTPS;
    unsigned char bSE;
} ECU;

typedef struct {
  // struct ECU ecu;
  PDM pdm;
  ECU ecu;
} CANData;


CANData can;

// PDM:
IDTuple battVolt;
IDTuple fPCurr;
IDTuple fPVolt;
IDTuple fpStatusField;
IDTuple kSState;
IDTuple tBCurr;
IDTuple tBVolt;
IDTuple tBStatusFault;
IDTuple tBStatusField;
IDTuple cF1Status;
IDTuple cF2Status;
IDTuple fICurr;
IDTuple fIVolt;
IDTuple fIStatusField;
IDTuple sRStatus;

//ECU:
IDTuple rPM;
IDTuple tTPS;
IDTuple bSE;


void setup() {
    //PDM:
    // MESSAGE:0x600
    battVolt.address = 0x600; battVolt.channel = 1;
    fPCurr.address = 0x600; fPCurr.channel = 2; 
    fPVolt.address = 0x600; fPVolt.channel = 3; 
    fpStatusField.address = 0x600; fpStatusField.channel = 4; 
    kSState.address = 0x600; kSState.channel = 5; 
    tBCurr.address = 0x600; tBCurr.channel = 6;
    tBVolt.address = 0x600; tBVolt.channel = 7;
    // MESSAGE: x601
    tBStatusFault.address = 0x601; tBStatusFault.channel = 0;
    tBStatusField.address = 0x601; tBStatusField.channel = 1;
    cF1Status.address = 0x601; cF1Status.channel = 2;
    cF2Status.address = 0x601; cF2Status.channel = 3;
    fICurr.address = 0x601; fICurr.channel = 4;
    fIVolt.address = 0x601; fIVolt.channel = 5;
    fIStatusField.address = 0x601; fIStatusField.channel = 6;
    sRStatus.address = 0x601; sRStatus.channel = 7;

    //ECU:
    // MESSAGE: XXX
    // rPM.address = 0xXXX; rPM.channel = X;
    // tTPS.address = 0xXXX; tTPS.channel = X;
    // bSE.address = 0xXXX; bSE.channel = X;



    SERIAL_PORT_MONITOR.begin(115200);

    while (CAN_OK != CAN.begin(CAN_1000KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}


void loop() {
    unsigned char len = 0;
    unsigned char buf[8];

    if (CAN_MSGAVAIL == CAN.checkReceive()) {         // check if data coming
        CAN.readMsgBuf(&len, buf);                    // read data,  len: data length, buf: data buf
        unsigned long canId = CAN.getCanId();         // read canID
        
        //PDM:
        if (canId == battVolt.address) {
          // can.pdm.battVolt = buf[battVolt.channel] * 0.1216;  
          can.pdm.battVolt = buf[battVolt.channel];  
        }
        if (canId == fPCurr.address) {
          can.pdm.fPCurr = buf[fPCurr.channel];  
        }
        if (canId == fPVolt.address) {
          can.pdm.fPVolt = buf[fPVolt.channel];  
        }
        if (canId == fpStatusField.address) {
          can.pdm.fpStatusField = buf[fpStatusField.channel];  
        }
        if (canId == kSState.address) {
          can.pdm.kSState = buf[kSState.channel];  
        }
        if (canId == tBCurr.address) {
          can.pdm.tBCurr = buf[tBCurr.channel];  
        }
        if (canId == tBVolt.address) {
          can.pdm.tBVolt = buf[tBVolt.channel];  
        }
        if (canId == tBStatusFault.address) {
          can.pdm.tBStatusFault = buf[tBStatusFault.channel];  
        }
        if (canId == tBStatusField.address) {
          can.pdm.tBStatusField = buf[tBStatusField.channel];  
        }
        if (canId == cF1Status.address) {
          can.pdm.cF1Status = buf[cF1Status.channel];  
        }
        if (canId == cF2Status.address) {
          can.pdm.cF2Status = buf[cF2Status.channel];  
        }
        if (canId == fICurr.address) {
          can.pdm.fICurr = buf[fICurr.channel];  
        }
        if (canId == fIVolt.address) {
          can.pdm.fIVolt = buf[fIVolt.channel];  
        }
        if (canId == fIStatusField.address) {
          can.pdm.fIStatusField = buf[fIStatusField.channel];  
        }
        if (canId == sRStatus.address) {
          can.pdm.sRStatus = buf[sRStatus.channel];  
        }

        // ECU:
        // if (canId == rPM.address) {
        //   can.ecu.rPM = buf[rPM.channel];  
        // }
        // if (canId == tTPS.address) {
        //   can.ecu.tTPS = buf[tTPS.channel];  
        // }
        // if (canId == bSE.address) {
        //   can.ecu.bSE = buf[bSE.channel];  
        // }

        printData();    
     }  
  }


void printData(){
    SERIAL_PORT_MONITOR.println("========================");
    //PDM
    SERIAL_PORT_MONITOR.print("battVolt: ");
    SERIAL_PORT_MONITOR.println(can.pdm.battVolt);
    SERIAL_PORT_MONITOR.print("fPCurr: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fPCurr);
    SERIAL_PORT_MONITOR.print("fPVolt: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fPVolt);
    SERIAL_PORT_MONITOR.print("fpStatusField: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fpStatusField);
    SERIAL_PORT_MONITOR.print("kSState: ");
    SERIAL_PORT_MONITOR.println(can.pdm.kSState);
    SERIAL_PORT_MONITOR.print("tBCurr: ");
    SERIAL_PORT_MONITOR.println(can.pdm.tBCurr);
    SERIAL_PORT_MONITOR.print("tBVolt: ");
    SERIAL_PORT_MONITOR.println(can.pdm.tBVolt);
    SERIAL_PORT_MONITOR.print("tBStatusFault: ");
    SERIAL_PORT_MONITOR.println(can.pdm.tBStatusFault);
    SERIAL_PORT_MONITOR.print("tBStatusField: ");
    SERIAL_PORT_MONITOR.println(can.pdm.tBStatusField);
    SERIAL_PORT_MONITOR.print("cF1Status: ");
    SERIAL_PORT_MONITOR.println(can.pdm.cF1Status);
    SERIAL_PORT_MONITOR.print("cF2Status: ");
    SERIAL_PORT_MONITOR.println(can.pdm.cF2Status);
    SERIAL_PORT_MONITOR.print("fICurr: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fICurr);
    SERIAL_PORT_MONITOR.print("fIVolt: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fIVolt);
    SERIAL_PORT_MONITOR.print("fIStatusField: ");
    SERIAL_PORT_MONITOR.println(can.pdm.fIStatusField);
    SERIAL_PORT_MONITOR.print("sRStatus: ");
    SERIAL_PORT_MONITOR.println(can.pdm.sRStatus);

    //ECU
    // SERIAL_PORT_MONITOR.print("rPM: ");
    // SERIAL_PORT_MONITOR.println(can.ecu.rPM);
    // SERIAL_PORT_MONITOR.print("tTPS: ");
    // SERIAL_PORT_MONITOR.println(can.ecu.tTPS);
    // SERIAL_PORT_MONITOR.print("bSE: ");
    // SERIAL_PORT_MONITOR.println(can.ecu.bSE);
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
