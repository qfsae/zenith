#include "Arduino.h"
#include <cal.hpp>
#include <can.hpp>

// Acquired from looking at the arduino to STM pin mapping
// https://github.com/stm32duino/Arduino_Core_STM32/blob/master/variants/Generic_F446Rx/variant.h
// NOTE: Arduino layer in platformio supports CMSIS mapping of MCU registers
// Thus, all the low level code we write will be directly portable to the Arduino environment in platformio
#define LED A8

CAL::CAL cal;

void setup() {
	Serial.begin(9600);
	
	bool ret = CANInit(CAN_500KBPS, 0, 2);
	while(!ret);
	Serial.println("CAN Initialized");
	cal.updateVar(CAL::DATA_DASH::DownShift, 0);
	cal.updateVar(CAL::DATA_DASH::UpShift, 0);
}

CAL::CAN_msg_t can_msg;


uint8_t can_ch = 1;
int timer;

void loop() {
	if(CANMsgAvail(can_ch)){
		CANReceive(can_ch, &can_msg);
		cal.updatePackage(can_msg);
	}
	if(cal.returnVar(CAL::DATA_ECU::EngineRPM) > 12000){
		if(cal.returnVar(CAL::DATA_DASH::UpShift) == 1){
			cal.updateVar(CAL::DATA_ECU::EngineRPM, 3500);
			cal.updateVar(CAL::DATA_DASH::UpShift, 0);
		}
	}
	if(timer>100){
		cal.updateVar(CAL::DATA_ECU::EngineRPM, (cal.returnVar(CAL::DATA_ECU::EngineRPM) + 100));
		timer = 0;
	}
	timer+=20;
	CANSend(can_ch, &cal.package(CAL::DATA_ECU::EngineRPM)); 

	delay(20);
}
