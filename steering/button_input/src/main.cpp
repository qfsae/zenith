#include "Arduino.h"

int button = 0;
long int start = 0;
long int end = 0;
long int elapsed = 0;

void timing(){
    if (button == 1){
        start = millis();
    } else {
        end = millis();
        elapsed = end - start;
        //to avoid a bunch of zeroes printed to system
        if (elapsed != 0 ) {Serial.println(elapsed);}
    }
}
 
void setup() {
    // Initialize the Serial bus at 115.2 kBaud
    Serial.begin(115200);

    // pin 7 maps to signal input on nucleo
    pinMode(7, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(7), timing, CHANGE);
}

void loop() {
    button = digitalRead(7);
}

