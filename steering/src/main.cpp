/**
 * @file main.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @author Logan Lim
 * @author Ethan Peterson (portfolio.petetech.net)
 * @brief Q22 QFSAE Steering Wheel Code
 * @version 1.2
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Arduino.h>
#include <SPI.h>
#include "EVE_target.hpp"
#include "EVE_commands.hpp"
#include "steering_io.h"
#include "EasyButton.h"
#include "display/display.hpp"
#include "main.hpp"

// Blink Rate of display elements
#define DISPLAY_BLINK_TIME 1000 //(ms)
#define ENGINE_SHIFT_DEBOUNCE 1000 //(ms)

#define UPSHIFT_DEBOUNCE_TIME 35
#define SHIFT_PULLUP_EN false // Pull up is provided on the physical PCB
#define SHIFT_ACTIVE_LOW true

EasyButton upshiftButton(STEERING_DOWNSHIFT, UPSHIFT_DEBOUNCE_TIME, SHIFT_PULLUP_EN, SHIFT_ACTIVE_LOW);
EasyButton downshiftButton(STEERING_UPSHIFT, UPSHIFT_DEBOUNCE_TIME, SHIFT_PULLUP_EN, SHIFT_ACTIVE_LOW);

CAL::CAL cal;
Display tft;

bool upshift = false;
bool downshift = false;

void upshift_handler() {
    upshift = true;
    if (tft.gear < 5) {
        tft.gear++;
    }
}

void downshift_handler() {
    downshift = true;
    if (tft.gear > 0) {
        tft.gear--;
    }
}

void setup() {
    
    tft.gear = 0;
    upshiftButton.begin(); 
    upshiftButton.onPressed(upshift_handler);

    downshiftButton.begin();
    downshiftButton.onPressed(downshift_handler);

    // Set up Steering Wheel CAN (CAN controller needs enable)
    pinMode(STEERING_CAN_OE, OUTPUT);
    digitalWrite(STEERING_CAN_OE, LOW);


    // Pull steering pins up
    pinMode(STEERING_DOWNSHIFT, INPUT_PULLUP);
    pinMode(STEERING_UPSHIFT, INPUT_PULLUP);

    Serial2.begin(115200);

    tft.setup();

    // Initialize CAN BUS
	if (CANInit(CAN_1000KBPS, 0, 2)) {
        Serial2.println("CAN BUS UP!");
        tft.CAN_Init_Error = false;
    } else {
        Serial2.println("CAN INIT FAIL");
        tft.CAN_Init_Error = true;
    }
    // Display splash logo
    tft.display(Display::Screens::Splash);
    delay(2000);
}

uint8_t can_ch1 = 1;
// CAN message intermediary (need to make cal update package inline)
CAN_msg_t can_msg;

// Shift Timers
uint32_t dtimer = 0;
uint32_t utimer = 0;


void loop() {

    upshiftButton.read(); // call the polling updater in the library
    downshiftButton.read(); // call the polling updater in the library

    // Check for CAN message
    if(CANMsgAvail(can_ch1)) {
        CANReceive(can_ch1, &can_msg);
        // Update CAL stored messages
        cal.updatePackage(can_msg); // TODO: make inline
        // Update time since last CAN recv
        tft.updateCAN();
	}

    // Display runtime call
    tft.display(Display::Screens::Main/*Display main screen (gear, speed, rpm, engine temp) */);


    // BEGIN shiftingLogic

        // Reset timers on shift
        if(downshift == true){
            downshift = false;
            dtimer = millis();
        }

        if(upshift == true){
            upshift = false;
            utimer = millis();
        }

        // Send positive shift for time allotment (1000_ms) should probably be set to slightly above engine debounce
        if(downshift == false && (millis()-dtimer) > ENGINE_SHIFT_DEBOUNCE){
            //  5000 is 5 volts (ECU off)
            cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset0, ECU5V);
        }
        else{
            cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset0, ECU0V);
            downshift = false;
        }

        if(upshift  == false && (millis()-utimer) > ENGINE_SHIFT_DEBOUNCE){
            cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset1, ECU5V);
        }
        else{
            cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset1, ECU0V);
            upshift = false;
        }

    // END shiftingLogic

    // Send out persistent updates to ECU (else it complains and freaks out)
    // Automatic retransmission must be disabled within `st-f4can`
    //      Failure bricks steering wheel (msg outbox fills up {possible memory perm err?})
    CANSend(can_ch1, &cal.package(CAL::MOTEC_RECV_ID::ECU_CAN6));

}
