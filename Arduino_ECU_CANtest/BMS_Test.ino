#include <SPI.h>
#include "mcp2515_can.h"

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

/*


Byte 0: Cell ID (8 bit, starting with 0)
Byte 1&2: Instant Voltage (16 bit, unit: 0.1mv)
Byte 3&4: Internal Resistance (15 bit, unit: 0.01mOhm)
Byte 5&6: Open Voltage (16 bit, unit: 0.1mv)
Byte 7: Checksum (8 bit)

Bit 8 in byte #3 is whether or not the cell is shunting 
(1 indicates current is being shunted, 0 means it is not).

*/

const int SPI_CS_PIN = 9;
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin

//variables for CAN data
uint8_t cell_id;
uint16_t inst_volt;
uint16_t internal_resistance; 
uint16_t open_volt;
uint8_t checksum;

void setup() {
    SERIAL.begin(115200);
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {             // init can bus : baudrate = 500k
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
}

uint8_t stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop() {
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    cell_id = 0xA0;
    inst_volt = 0x3C69;
    internal_resistance = 0x7269;
    open_volt = 0x7269;
    checksum = 0xA0;

    calc_checksum 

    stmp[0] = cell_id;
    stmp[1] = inst_volt; 
    stmp[2] = inst_volt << 4;
    stmp[3] = internal_resistance;
    stmp[4] = internal_resistance << 4;
    stmp[5] = open_volt;
    stmp[6] = open_volt << 4;
    stmp[7] = checksum;

    CAN.sendMsgBuf(0x00, 0, 8, stmp);
    delay(100);                       // send data per 100ms
}


