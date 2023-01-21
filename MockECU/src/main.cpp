#include "Arduino.h"
#include "can.hpp"
#include "cal.hpp"

// Acquired from looking at the arduino to STM pin mapping
// https://github.com/stm32duino/Arduino_Core_STM32/blob/master/variants/Generic_F446Rx/variant.h
// NOTE: Arduino layer in platformio supports CMSIS mapping of MCU registers
// Thus, all the low level code we write will be directly portable to the Arduino environment in platformio
#define LED A8

CAL::CAL cal1;
CAL::CAL cal2;


void setup() {	
	bool ret = CANInit(CAN_500KBPS, 0, 2);
	while(!ret);
	cal1.updateVar(CAL::DATA_DASH::DownShift, 0);
	cal1.updateVar(CAL::DATA_DASH::UpShift, 0);
	cal2.updateVar(CAL::DATA_DASH::DownShift, 0);
	cal2.updateVar(CAL::DATA_DASH::UpShift, 0);
	pinMode(A8, OUTPUT);
	digitalWrite(A8, 0);
}

CAN_msg_t can_msg1;
CAN_msg_t can_msg2;


uint8_t can_ch1 = 1;
uint8_t can_ch2 = 2;
int timer;
int rpm = 0;
int redLine = 9600;
int coolant_temp = 50;
int engine_temp = 69;
int speed = 0;
int tps;
void loop() {
	cal2.updateVar(CAL::DATA_ECU::EngineRPM, rpm);
	cal2.updateVar(CAL::DATA_ECU::EngineTemp, engine_temp);
	cal2.updateVar(CAL::DATA_ECU::CoolantTemp, coolant_temp);
	cal2.updateVar(CAL::DATA_ECU::VehicleSpeed, speed);
	cal2.updateVar(CAL::DATA_ECU::ThrottlePosition, tps);

	CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::EngineRPM));
	CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::EngineTemp));
	CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::CoolantTemp));
	CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::VehicleSpeed));
	CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::ThrottlePosition));

	delay(100);
	if (rpm > redLine) {
		rpm = 0;
		speed = 0;
	} else {
		rpm += 147;
		speed += 3;
	}

	if (tps == 100) {
		tps = 0;
	} else {
		tps++;
	}
}
