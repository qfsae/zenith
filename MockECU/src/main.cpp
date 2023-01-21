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

void loop() {
	if(CANMsgAvail(can_ch1)){
		CANReceive(can_ch1, &can_msg1);
		cal1.updatePackage(can_msg1);
	}
	if(cal1.returnVar(CAL::DATA_ECU::EngineRPM) > 12100){
		if(cal1.returnVar(CAL::DATA_DASH::UpShift) == 1){
			cal1.updateVar(CAL::DATA_ECU::EngineRPM, 3500);
			cal1.updateVar(CAL::DATA_DASH::UpShift, 0);
		}
	}
	if(timer>100){
		cal1.updateVar(CAL::DATA_ECU::EngineRPM, (cal1.returnVar(CAL::DATA_ECU::EngineRPM) + 100));
		cal2.updateVar(CAL::DATA_ECU::EngineRPM, (cal2.returnVar(CAL::DATA_ECU::EngineRPM) + 100));
		timer = 0;
	}
	if(CANMsgAvail(can_ch2)){
		CANReceive(can_ch2, &can_msg2);
		cal2.updatePackage(can_msg2);
	}
	if(cal2.returnVar(CAL::DATA_ECU::EngineRPM) > 12100){
		if(cal2.returnVar(CAL::DATA_DASH::UpShift) == 1){
			cal2.updateVar(CAL::DATA_ECU::EngineRPM, 3500);
			cal2.updateVar(CAL::DATA_DASH::UpShift, 0);
		}
	}
	else{
		//CANSend(can_ch1, &cal1.package(CAL::DATA_ECU::EngineRPM));
		CANSend(can_ch2, &cal2.package(CAL::DATA_ECU::EngineRPM));
		timer+=100;
		delay(100);
	}
	timer+=20;
	delay(20);
}
