#include <Arduino.h>
#include <SPI.h>

#include "steering_io.h"

#include "cal.hpp"
#include "can.hpp"

#include "tft.h"

#include "screens.h"

// Only do print statements if we are in debug mode
#define STEERING_DEBUG 1

void setup() {
    Serial2.begin(115200);
    pinMode(EVE_CS, OUTPUT);
    digitalWrite(EVE_CS, HIGH);
    pinMode(EVE_PDN, OUTPUT);
    digitalWrite(EVE_PDN, LOW);

    pinMode(STEERING_CAN_OE, OUTPUT);
    digitalWrite(STEERING_CAN_OE, LOW);

    bool ret = CANInit(CAN_1000KBPS, 0, 2);
    if (!ret) while (true);

    SPI.begin(); // Set up the SPI to run in Mode 0 and 8 MHz
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    TFT_init();

    // Display splash logo
    TFT_splash();
    delay(3000);
}

void loop() {
    static uint32_t previous_millis = 0;
    static uint8_t display_delay = 0;
    uint32_t current_millis;
    current_millis = millis();

    if ((current_millis - previous_millis) > 3) {
        // Execute touch display code every ~5 ms
        previous_millis = current_millis;

        TFT_touch();

        display_delay++;
        if (display_delay > 3) { // Execute display refresh every ~20ms
            TFT_display((tft_callback)display_code);
        }
    }

    uint8_t recv_ch = 1;
    CAN_msg_t CAN_RX_msg;
    if (CANMsgAvail(recv_ch)) {
        CANReceive(recv_ch, &CAN_RX_msg);
        Serial2.print("ID: ");
        Serial2.println(CAN_RX_msg.id, HEX);
        for (int i = 0; i < CAN_RX_msg.len; i++) {
            Serial2.print(CAN_RX_msg.data[i]);
            Serial2.print("\t");
        }

        Serial2.print("\n");
        if (CAN_RX_msg.id == 0x118) {
            // tps = CAN_RX_msg.data[1];
        }
    }
}
