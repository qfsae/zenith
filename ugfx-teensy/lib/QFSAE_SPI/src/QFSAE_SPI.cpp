#include "QFSAE_SPI.h"
#include "SPI.h"

using namespace std;

    uint8_t QSPI::rd8(uint32_t addr) {
        sel();
        uint8_t read = 0x00;

        uint8_t tx[] = {
            (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            0x00                            // dummy byte
        };

        SPI.transfer(tx[0]);
        SPI.transfer(tx[1]); 
        SPI.transfer(tx[2]);
        SPI.transfer(tx[3]);
        uint8_t byte1 = SPI.transfer(0x00);

        read = byte1;
        desel();
        return read;
    }
    uint16_t QSPI::rd16(uint32_t addr) {
        sel();
        uint16_t read = 0x0000;
        
        uint8_t tx[] = {
            (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            0x00                            // dummy byte
        };

        SPI.transfer(tx[0]);
        SPI.transfer(tx[1]); 
        SPI.transfer(tx[2]);
        SPI.transfer(tx[3]);
        uint8_t byte1 = SPI.transfer(0x00);
        uint8_t byte2 = SPI.transfer(0x00);
        
        read = (byte2<<8) | byte1;
        desel();
        return read;
    }
    uint32_t QSPI::rd32(uint32_t addr) {
        sel();
        uint32_t read = 0x00000000;

        uint8_t tx[] = {
            (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            0x00                            // dummy byte
        };

        SPI.transfer(tx[0]);
        SPI.transfer(tx[1]); 
        SPI.transfer(tx[2]);
        SPI.transfer(tx[3]);
        uint8_t byte1 = SPI.transfer(0x00);
        uint8_t byte2 = SPI.transfer(0x00);
        uint8_t byte3 = SPI.transfer(0x00);
        uint8_t byte4 = SPI.transfer(0x00);
        
        read = (byte4<<24) | (byte3<<16) | (byte2<<8) | byte1;
        desel();
        return read;
    }

    void QSPI::wr8(uint32_t addr, uint8_t data) {
        sel();
        uint8_t tx[4] = {
            (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            (data >> 0) & 0xFF              // first byte of data
        };

        for(int j = 0; j < 4; j++) {
            SPI.transfer(tx[j]);
        } 
        desel();
    }
    void QSPI::wr16(uint32_t addr, uint16_t data) {
        sel();
        uint8_t tx[5] = {
            (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            (data >> 0) & 0xFF,             // first byte of data
            (data >> 8) & 0xFF              // second byte of data
        };

        for(int j = 0; j < 5; j++) {
            SPI.transfer(tx[j]);
        } 
        desel();
    }
    void QSPI::wr32(uint32_t addr, uint32_t data) {
        sel();
        uint8_t tx[7] = {
            (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
            (addr >> 8) & 0xFF,             // bits 8-15 of addr
            (addr >> 0) & 0xFF,             // bits 0-7 of addr
            (data >> 0) & 0xFF,            // first byte of data
            (data >> 8) & 0xFF,            // second byte of data
            (data >> 16) & 0xFF,             // third byte of data
            (data >> 24) & 0xFF              // fourth byte of data
        };

        for(int j = 0; j < 8; j++) {
            SPI.transfer(tx[j]);
        } 
        desel();
    }

    void QSPI::command(uint32_t cmd) {
        sel();
        uint8_t tx[] = {
            (0x40 | ((cmd >> 16) & 0xFF)),  // cmd code, command
            (cmd >> 8) & 0xFF,              // parameter
            0x00                            // blank byte
        };

        SPI.transfer(tx[0]);
        SPI.transfer(tx[1]);
        SPI.transfer(tx[2]);

        desel();
    }
