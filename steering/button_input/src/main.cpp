#include "Arduino.h"

unsigned long start = 0;
unsigned long end = 0;
unsigned long elapsed = 0;
int flag = 0;

//when button is 0 -> pressed

void startTiming(){
    start = millis();
    flag = 0;
}

void timer(){
    elapsed = end-start;
    if (elapsed > 1 && flag == 0) {
        Serial.println(elapsed);
        flag = 1;
    }
}

void endTiming(){
    end = millis();
    timer();
}

void ISR(){
    if (digitalRead(7) == LOW){
        startTiming();
    } else if (digitalRead(7) == HIGH){
        endTiming();
    }
}
 
void setup() {
    // Initialize the Serial bus at 115.2 kBaud
    Serial.begin(115200);

    // pin 7 maps to signal input on nucleo
    pinMode(7, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(7), ISR, CHANGE);
}

void loop() { 
}

// 2 interrupts/button - RISING, FALLING
// ISR#1 - start timer
// ISR#2 - stop timer and leave space for function
// empty func - print stmt millis