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

// display
#include "EVE_target.hpp"
#include "EVE_commands.hpp"

// my libs
#include "cal.hpp"
#include "can.hpp"

// ethan libs
#include "steering_io.h"
#include "EasyButton.h"

// includes
#include "display/display.hpp"

// Blink Rate of display elements
#define DISPLAY_BLINK_TIME 1000 //(ms)

#define UPSHIFT_DEBOUNCE_TIME 35
#define UPSHIFT_PULLUP_EN false // Pull up is provided on the physical PCB
#define UPSHIFT_ACTIVE_LOW true

EasyButton upshiftButton(STEERING_BUTTON_3, UPSHIFT_DEBOUNCE_TIME, UPSHIFT_PULLUP_EN, UPSHIFT_ACTIVE_LOW);
EasyButton downshiftButton(STEERING_BUTTON_4, UPSHIFT_DEBOUNCE_TIME, UPSHIFT_PULLUP_EN, UPSHIFT_ACTIVE_LOW);

CAL::CAL cal;
Display tft;

void upshift_handler() {
    Serial2.println("upshift!");
    if (tft.gear < 5) {
        tft.gear++;
    }
}

void downshift_handler() {
    Serial2.println("downshift!");
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

    Serial2.begin(115200);

    tft.setup();

    // Initialize CAN BUS
	if (CANInit(CAN_500KBPS, 0, 2)) {
        Serial2.println("CAN BUS UP!");
    } else {
        Serial2.println("CAN INIT FAIL");
    }
    // Display splash logo
    tft.display(Display::Screens::Splash);
    delay(500);
}

uint8_t can_ch1 = 1;
CAN_msg_t can_msg;

void loop() {
    upshiftButton.read(); // call the polling updater in the library
    downshiftButton.read(); // call the polling updater in the library
    if(CANMsgAvail(can_ch1)) {
        CANReceive(can_ch1, &can_msg);
        cal.updatePackage(can_msg);
        tft.updateCAN();
	}
    tft.display(Display::Screens::Main);
}
