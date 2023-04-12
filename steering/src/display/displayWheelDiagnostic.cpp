/**
 * @file display.cpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "display/display.hpp"
#include "SPI.h"
#include "EVE.hpp"
#include "display/tft_data.hpp"
#include "display/consts.hh"
#include "main.hpp"
#include "steering_io.h"
#include <sstream>

#define FLOAT_STRING(f) (std::to_string((int)f) + "." + std::to_string((int)((f-(float)((int)f))*100.0)))

void Display::displayWheelDiagnostic(){
    if(tft_active != 0) {
        if(EVE_IS_BUSY == EVE_busy()) { // Is EVE still processing the last display list?
            //display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */
            return;
        }

    // Start refresh init

        EVE_start_cmd_burst();          // Start writing to the cmd-fifo as one stream of bytes, only sending the address once
        EVE_cmd_dl_burst(CMD_DLSTART);  // Start the display list
        EVE_cmd_dl_burst(DL_COLOR_RGB | color::Black);  // Set the default clear color to black
        EVE_cmd_dl_burst(DL_CLEAR |     // Clear the screen - this and the previous prevent artifacts between list
                        CLR_COL | // Clear the display's bg (background) color
                        CLR_STN | // Clear Stencil
                        CLR_TAG); // Clear tag buffer
        EVE_cmd_dl_burst(TAG(0));

        EVE_cmd_dl_burst(VERTEX_FORMAT(0)); // Tell the display its about to get a bunch of commands

    // End refresh init commands
    // Start Display write

////////////////////////////////////////////////// begin Button Checkers
        if(digitalRead(STEERING_BUTTON_1)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(0,0,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 70, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(0,0,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 70, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(50, 35, fonts::regularLarge, EVE_OPT_CENTER, "SB1");

        if(digitalRead(STEERING_BUTTON_3)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(0,70,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 140, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(0,70,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 140, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(50, 105, fonts::regularLarge, EVE_OPT_CENTER, "SB3");

        if(digitalRead(STEERING_DOWNSHIFT)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(0,140,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 210, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(0,140,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 210, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(50, 175, fonts::regularLarge, EVE_OPT_CENTER, "SFTDN");

        if(cal.returnVar(CAL::DATA_ECU::GearNeutralSwitch)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(0,210,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 250, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(0,210,0,0));
            EVE_cmd_dl_burst(VERTEX2II(100, 250, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(50, 230, fonts::error, EVE_OPT_CENTER, "GNS");

/////////////////////////////////////////////////////////////////////////////////////////
        if(digitalRead(STEERING_BUTTON_4)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(380,0,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 70, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(380,0,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 70, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(430, 35, fonts::regularLarge, EVE_OPT_CENTER, "SB4");

        if(digitalRead(STEERING_BUTTON_2)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(380,70,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 140, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(380,70,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 140, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(430, 105, fonts::regularLarge, EVE_OPT_CENTER, "SB2");

        if(cal.returnVar(CAL::DATA_ECU::ClutchState)){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(380,140,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 210, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(380,140,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 210, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(430, 175, fonts::regularLarge, EVE_OPT_CENTER, "Clutch");

        if(Launch_Control_en){
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Green);
            EVE_cmd_dl_burst(VERTEX2II(380,210,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 250, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        else{
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            EVE_color_rgb_burst(color::Red);
            EVE_cmd_dl_burst(VERTEX2II(380,210,0,0));
            EVE_cmd_dl_burst(VERTEX2II(480, 250, 0,0));
            EVE_cmd_dl_burst(DL_END);
        }
        EVE_color_rgb_burst(color::Black);
        EVE_cmd_text_burst(430, 230, fonts::error, EVE_OPT_CENTER, "LAUNCH");
////////////////////////////////////////////////// end Button Checkers

        // Car Info
            EVE_color_rgb_burst(color::White);
            float vtemp;
            cal.returnVar(CAL::DATA_PDM::BatteryVoltage, vtemp);
            EVE_cmd_text_burst(240, 50, fonts::regularLarge, EVE_OPT_CENTER, "Battery Voltage:");
            EVE_cmd_text_burst(240, 80, fonts::regularLarge, EVE_OPT_CENTER,  FLOAT_STRING(vtemp).c_str());


            EVE_cmd_text_burst(240, 120, fonts::regularLarge, EVE_OPT_CENTER, ("TPS:   " + std::to_string(cal.returnVar(CAL::DATA_ECU::ThrottlePosition)) + "%").c_str());

            cal.returnVar(CAL::DATA_PDM::GearPositionVoltage, vtemp);
            EVE_cmd_text_burst(240, 160, fonts::regularLarge, EVE_OPT_CENTER, "GearPos Voltage:");
            EVE_cmd_text_burst(240, 190, fonts::regularLarge, EVE_OPT_CENTER,  FLOAT_STRING(vtemp).c_str());


        // Error Write
        if(checkErrors()!=""){
            if(clock){
                EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
                EVE_color_rgb_burst(color::Red);
                EVE_cmd_dl_burst(VERTEX2II(0,250,0,0));
                EVE_cmd_dl_burst(VERTEX2II(500, 300, 0,0));
                EVE_cmd_dl_burst(DL_END);
                EVE_color_rgb_burst(color::Black);
            }
            else {EVE_color_rgb_burst(color::White);}
            EVE_cmd_text_burst(coordinates::errorText::x, coordinates::errorText::y, fonts::error, EVE_OPT_CENTER, checkErrors().c_str());
        }
        // End Error Write
    
    // End Display Write



        /* DYNAMIC DISPLAY CODE HERE */
        
        EVE_cmd_dl_burst(DL_DISPLAY); // Instruct the co-processor to show the list 
        EVE_cmd_dl_burst(CMD_SWAP); // Make this list active

        EVE_end_cmd_burst(); // Stop writing to the cmd-fifo, the cmd-FIFO will be executed automatically after this or when DMA is done
    }
}