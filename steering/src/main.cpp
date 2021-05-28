#include "Arduino.h"
#include "steering_io.h"

#define LED 29        // status LED

int ledState = 0;

void setup() {
  Serial2.begin(115200);
  pinMode(LED, OUTPUT);           // status LED output
}

void loop()
{
  ledState = !ledState;
  digitalWrite(LED, ledState);
  delay(200);
}
