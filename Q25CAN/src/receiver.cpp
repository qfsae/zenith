// src/receiver.cpp
#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>
#include <string.h>
#include "cal.hpp"

// Define the chip select pin for the MCP2515 CAN shield
static const int CS_PIN = 9;

// Internal linkage instances
static MCP2515 mcp2515(CS_PIN);
static CAL::CAL  cal;

void receiver_setup() {
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial monitor

  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  Serial.println("Receiver setup complete.");
}

void receiver_loop() {
  struct can_frame frame;
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    // Build a clean CAN_msg_t
    CAL::CAN_msg_t msg;

    // 29‑bit identifier
    msg.id = frame.can_id;

    // Data payload: zero first, then copy actual bytes
    memset(msg.data, 0, sizeof(msg.data));
    memcpy(msg.data, frame.data, frame.can_dlc);

    // Number of valid data bytes
    msg.len    = frame.can_dlc;

    // These fields are unused or fixed for our setup
    msg.ch     = 0;    // unused channel
    msg.format = 0;    // 0 = STANDARD (11‑bit ID)
    msg.type   = 0;    // 0 = DATA FRAME (not remote)

    // Hand off to CAL for storage & parsing
    cal.updatePackage(msg);
  }
}