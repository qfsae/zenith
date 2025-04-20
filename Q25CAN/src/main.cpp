#include <Arduino.h>

// Define device modes
#define DEVICE_MODE_SENDER   1
#define DEVICE_MODE_RECEIVER 2

// Set the DEVICE_MODE macro.
// Change this definition to switch between sender and receiver code.
#define DEVICE_MODE DEVICE_MODE_RECEIVER
// #define DEVICE_MODE DEVICE_MODE_RECEIVER  // Uncomment for receiver mode

// Declare external functions for sender and receiver.
#if (DEVICE_MODE == DEVICE_MODE_SENDER)
  extern void sender_setup();
  extern void sender_loop();
#elif (DEVICE_MODE == DEVICE_MODE_RECEIVER)
  extern void receiver_setup();
  extern void receiver_loop();
#else
  #error "Invalid DEVICE_MODE selected. Choose either DEVICE_MODE_SENDER or DEVICE_MODE_RECEIVER."
#endif

void setup() {
  #if (DEVICE_MODE == DEVICE_MODE_SENDER)
    sender_setup();
  #elif (DEVICE_MODE == DEVICE_MODE_RECEIVER)
    receiver_setup();
  #endif
}

void loop() {
  #if (DEVICE_MODE == DEVICE_MODE_SENDER)
    sender_loop();
  #elif (DEVICE_MODE == DEVICE_MODE_RECEIVER)
    receiver_loop();
  #endif
}
