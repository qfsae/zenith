#include <Arduino.h>
#include <SPI.h>

#include "steering_io.h"
#include "cal.hpp"
#include "can.hpp"

int ledState = 0;

void setup() {
  Serial2.begin(115200);
  pinMode(STEERING_STATUS_LED, OUTPUT);           // status LED output
}

void loop() {
  ledState = !ledState;
  digitalWrite(STEERING_STATUS_LED, ledState);
  delay(200);
}
