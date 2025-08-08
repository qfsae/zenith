// #include <Arduino.h>
// #include <SPI.h>
// #include <mcp2515.h>
// #include "cal.hpp"
// // Define the chip select pin for the MCP2515 CAN shield
// const int CS_PIN = 10;

// // Mark these as static so they have internal linkage.
// static MCP2515 mcp2515(CS_PIN);
// static CAL::CAL cal;

// void sender_setup() {
//   Serial.begin(9600);
//   while (!Serial);  // Wait for Serial monitor

//   // Initialize SPI bus
//   SPI.begin();

//   // Reset and configure the MCP2515 CAN controller
//   mcp2515.reset();
//   // Set the bitrate to 500kbps with a 16MHz oscillator (Arduino Uno)
//   mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
//   mcp2515.setNormalMode();

//   Serial.println("Sender setup complete.");
// }

// void sender_loop() {
//   // Simulate a PDM sensor value (for example, an analog reading)
//   int pdmValue = analogRead(A0);

//   // Update the CAL package for PDM_1 with the simulated value.
//   // (CAL::DATA_PDM_SIGNAL is defined in data_pdm.hpp)
//   cal.updateVar(CAL::DATA_PDM_SIGNAL, pdmValue);

//   // Retrieve the updated CAN message for PDM_1
//   CAL::CAN_msg_t &msg = cal.package(CAL::CAN_ID::PDM_1);

//   // Build a CAN frame from the CAL message
//   struct can_frame frame;
//   frame.can_id  = msg.id;
//   frame.can_dlc = msg.len;
//   for (uint8_t i = 0; i < msg.len; i++) {
//     frame.data[i] = msg.data[i];
//   }

//   // Send the CAN message over the bus
//   if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
//     Serial.print("Sent PDM value: ");
//     Serial.println(pdmValue);
//   } else {
//     Serial.println("Error sending CAN message");
//   }

//   delay(1000);
// }
