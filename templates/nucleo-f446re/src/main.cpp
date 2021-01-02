#include "Arduino.h"


void setup() {
    // Initialize the Serial bus at 115.2 kBaud
    Serial.begin(115200);

    // pin 13 maps to an on-board LED on the nucleo
    pinMode(13, OUTPUT);
}

void loop() {
    Serial.println("Hello World!");
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
}