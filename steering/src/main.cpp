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
 * 
 * Reference for timer interupt: https://github.com/stm32duino/STM32Examples/blob/main/examples/Peripherals/HardwareTimer/Timebase_callback/Timebase_callback.ino
 * 
 * st-f4CAN must be >=@^1.0.8
 * if st-f4CAN <=@^1.0.8 "Hardware initialization(With automatic retransmission)" must be exchanged for "Hardware initialization(Without automatic retransmission)"
 */

#include <Arduino.h>
#include <SPI.h>
#include "EVE_target.hpp"
#include "EVE_commands.hpp"
#include "steering_io.h"
#include "display/display.hpp"
#include "main.hpp"
#include <Adafruit_NeoPixel.h>

// Blink Rate of display elements
#define DISPLAY_BLINK_TIME 1000 //(ms)
#define ENGINE_SHIFT_DEBOUNCE 1000 //(ms)

#define SHIFT_DEBOUNCE_TIME 35
#define SHIFT_PULLUP_EN false // Pull up is provided on the physical PCB
#define SHIFT_ACTIVE_LOW true

#define BUTTON_DEBOUNCE_TIME 35
#define BUTTON_PULLUP_EN false
#define BUTTON_ACTIVE_LOW true

#define LAUNCH_CONTROL_DELAY 2000 //(ms)
#define NAV_PRESS_DELAY 1000 //(ms)

#define CAN_SEND_HZ 200 //(s^{-1})

//EasyButton upshiftButton(STEERING_UPSHIFT, SHIFT_DEBOUNCE_TIME, SHIFT_PULLUP_EN, SHIFT_ACTIVE_LOW);
//EasyButton downshiftButton(STEERING_DOWNSHIFT, SHIFT_DEBOUNCE_TIME, SHIFT_PULLUP_EN, SHIFT_ACTIVE_LOW);

EasyButton rightRed(STEERING_BUTTON_4, BUTTON_DEBOUNCE_TIME, BUTTON_PULLUP_EN, BUTTON_ACTIVE_LOW);
EasyButton rightBlue(STEERING_BUTTON_2, BUTTON_DEBOUNCE_TIME, BUTTON_PULLUP_EN, BUTTON_ACTIVE_LOW);
EasyButton leftRed(STEERING_BUTTON_1, BUTTON_DEBOUNCE_TIME, BUTTON_PULLUP_EN, BUTTON_ACTIVE_LOW);
EasyButton leftBlue(STEERING_BUTTON_3, BUTTON_DEBOUNCE_TIME, BUTTON_PULLUP_EN, BUTTON_ACTIVE_LOW);

CAL::CAL cal;
int logger_gear = 0;
Display tft;
// Current Screen Displayed
Display::Screens DispScrn = Display::Screens::Main;

uint8_t can_ch1 = 1;
// CAN message intermediary (need to make cal update package inline)
CAN_msg_t can_msg;


// NEOPIXELS
#define NUM_NEOPIXELS 16
Adafruit_NeoPixel pixels(NUM_NEOPIXELS, STEERING_NEOPIXEL, NEO_GRB + NEO_KHZ800);


void nextScreen(){
    if(DispScrn == Display::Screens::Main){
        DispScrn = Display::Screens::Splash;
    }
    else{
        DispScrn = (Display::Screens)((int)DispScrn + 1);
    }
}

void rightBlue_handler(){
    leftBlue.pressedFor(LAUNCH_CONTROL_DELAY);
}

void leftRed_handler(){ 
}

void leftBlue_handler(){
}

void sendCANMsg(void){
    // Send out persistent updates to ECU (else it complains and freaks out)
    // Automatic retransmission must be disabled within `st-f4can`
    //      Failure bricks steering wheel (msg outbox fills up {possible memory perm err?})
    CANSend(can_ch1, &cal.package(CAL::MOTEC_RECV_ID::ECU_CAN7));
}



void setup() {

    // begin Button setup
        rightRed.begin();
        //rightRed.onPressed(rightRed_handler);
        rightRed.onPressedFor(NAV_PRESS_DELAY, nextScreen);

        rightBlue.begin();
        rightBlue.onPressed(rightBlue_handler);
        leftRed.begin();
        leftRed.onPressed(leftRed_handler);

        leftBlue.begin();
        leftBlue.onPressed(leftBlue_handler);
    // end button setup

    // Set up Steering Wheel CAN (CAN controller needs enable)
    pinMode(STEERING_NEOPIXEL, OUTPUT);
    digitalWrite(STEERING_NEOPIXEL, LOW);

    pinMode(STEERING_CAN_OE, OUTPUT);
    digitalWrite(STEERING_CAN_OE, LOW);

    pixels.begin();

    // Pull steering pins up
    pinMode(STEERING_DOWNSHIFT, INPUT_PULLUP);
    pinMode(STEERING_UPSHIFT, INPUT_PULLUP);


    // NEOPIXEL
    pinMode(STEERING_TRANSLATOR_OE, OUTPUT);
    digitalWrite(STEERING_TRANSLATOR_OE, HIGH);
    
    Serial2.begin(115200);

    tft.setup();

    // Initialize CAN BUS
	if (CANInit(CAN_1000KBPS, 0, 2, false)) {
        Serial2.println("CAN BUS UP!");
        tft.CAN_Init_Error = false;
    } else {
        Serial2.println("CAN INIT FAIL");
        tft.CAN_Init_Error = true;
    }
    // Display splash logo
    tft.display(Display::Screens::Splash);

    // begin CAN timer interupt 
    #if defined(TIM1)
    TIM_TypeDef *Instance = TIM1;
    #else
    TIM_TypeDef *Instance = TIM2;
    #endif

    HardwareTimer * CANTimer = new HardwareTimer(Instance);

    CANTimer->setOverflow(CAN_SEND_HZ, HERTZ_FORMAT);
    CANTimer->attachInterrupt(sendCANMsg);
    //CANTimer->resume();
    // end CAN timer interupt
    delay(2000);
    
}

uint32_t ltimeout = 0;

void rpm_to_neopixels(int rpm) {
    // NOTE: we could add more here and handle a min / max rpm depending on the gear we are in

    // map the rpm value
    const int RPM_MIN = 2000;
    const int RPM_MAX = 9500;

    int pixels_enabled = map(rpm, RPM_MIN, RPM_MAX, 0, NUM_NEOPIXELS - 1);
    
    pixels.clear();
    // iterate over the LEDs in the order they are seen
    // green red blue
    for (int i = NUM_NEOPIXELS - 1; i >= NUM_NEOPIXELS - 1 - pixels_enabled; i--) {
        if (i > NUM_NEOPIXELS - 6) pixels.setPixelColor(i, pixels.Color(0, 64, 0));
        else if (i > NUM_NEOPIXELS - 11) pixels.setPixelColor(i, pixels.Color(64, 0, 0));
        else pixels.setPixelColor(i, pixels.Color(0, 0, 64));
    }
    pixels.show();
}

void loop() {

    // call the polling updater in the library
    rightRed.read();
    rightBlue.read();
    leftRed.read();
    leftBlue.read();

    // Check for CAN message
    if(CANMsgAvail(can_ch1)) {
        CANReceive(can_ch1, &can_msg);
        // Update CAL stored messages
        cal.updatePackage(can_msg); // TODO: make inline
        // Update time since last CAN recv
        tft.updateCAN();
        if(can_msg.id == 0x64D){
            logger_gear = can_msg.data[6] & 0x0F;
        };
	}

    // Display runtime call
    tft.display(DispScrn);

    rpm_to_neopixels(cal.returnVar(CAL::DATA_ECU::EngineRPM));
    // END shiftingLogic

    // begin launch control logic

        // if(tft.Launch_Control_en == true){
        //     cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset2, ECU5V);
        // }
        // else{
        //     cal.updateVar(CAL::DATA_ECU_RECV::ECU_CAN6::Offset2, ECU0V);
        // }

        // if(leftBlue.pressedFor(LAUNCH_CONTROL_DELAY) && rightBlue.pressedFor(LAUNCH_CONTROL_DELAY && ((ltimeout + 5000) < millis()))){
        //     tft.Launch_Control_en = !tft.Launch_Control_en;
        //     ltimeout = millis();
        // }

    // end launch control logic
}
