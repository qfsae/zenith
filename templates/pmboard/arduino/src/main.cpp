#include "Arduino.h"

// Acquired from looking at the arduino to STM pin mapping
// https://github.com/stm32duino/Arduino_Core_STM32/blob/master/variants/Generic_F446Rx/variant.h
// NOTE: Arduino layer in platformio supports CMSIS mapping of MCU registers
// Thus, all the low level code we write will be directly portable to the Arduino environment in platformio
#define LED A8

int ledState = 0;

void setup() {
	Serial.begin(9600);
	pinMode(LED, OUTPUT);
}

void loop() {
	Serial.println("Hello World!");
	delay(100);
	ledState = !ledState;
	digitalWrite(LED, ledState);
}
