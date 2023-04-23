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

// Memory-map defines
#define CENTER_N 0x0000f000 // start-address of n_center, needs 48672 bytes
#define CENTER_1 0x0001AE20 // start address of center_1, needs 48672 bytes
#define CENTER_2 0x00026C40 // start address of center_2, needs 48672 bytes
#define CENTER_3 0x00032A60 // start address of center_3, needs 48672 bytes
#define CENTER_4 0x0003E880 // start address of center_4, needs 48672 bytes
#define CENTER_5 0x0004A6A0 // start address of center_5, needs 48672 bytes

int get_gear(float pdm_reading) {
    // Can make these defines or CAL constants tossing em in for reference.
    const float NEUTRAL = 0.161*5.0;
    const float FIRST_PERCENT = 0.0*5.0;
    const float SECOND_PERCENT = 0.251*5.0;
    const float THIRD_PERCENT = 0.5*5.0;
    const float FOURTH_PERCENT = 0.751*5.0;
    const float FIFTH_PERCENT = 0.99*5.0;

    if (pdm_reading < 0.8)
        return 1;
    else if (pdm_reading > 0 && pdm_reading < 1.2)
        return 0;
    else if (pdm_reading > 1.2 && pdm_reading < 2)
        return 2;
    else if (pdm_reading > 2 && pdm_reading < 3)
        return 3;
    else if (pdm_reading > 3 && pdm_reading < 4)
        return 4;
    return 5; 
}

void Display::displayMain(){
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

            // Display the gear position
        EVE_cmd_dl_burst(DL_COLOR_RGB | color::White);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        switch(logger_gear)//get_gear(cal.returnVar_f(CAL::DATA_PDM::GearPositionVoltage)))
        {
            case 0:
                //                  Data address, img format, width, height (of image)
                EVE_cmd_setbitmap_burst(CENTER_N, EVE_ARGB1555, 156, 156); // displays N
                // Position on Screen ( x, y ) ( (0, 0) is top left)
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
            case 1:
                EVE_cmd_setbitmap_burst(CENTER_1, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
            case 2:
                EVE_cmd_setbitmap_burst(CENTER_2, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
            case 3:
                EVE_cmd_setbitmap_burst(CENTER_3, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
            case 4:
                EVE_cmd_setbitmap_burst(CENTER_4, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
            case 5:
                EVE_cmd_setbitmap_burst(CENTER_5, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(coordinates::gear::x, coordinates::gear::y));
                break;
        }
        EVE_cmd_dl_burst(DL_END);

        // Start RPM Display
        EVE_cmd_romfont(fonts::custom, fonts::large);
        EVE_cmd_number_burst(coordinates::rpm::x, coordinates::rpm::y, fonts::custom, EVE_OPT_CENTER, cal.returnVar(CAL::DATA_ECU::EngineRPM));
        // END RPM Display

        // Start KPH (Kilometers Per Hour) Display
        EVE_cmd_romfont(fonts::custom, fonts::large);
        EVE_cmd_text_burst(coordinates::speedText::x, coordinates::speedText::y, fonts::regularLarge, 0/*For none*/, "KPH");
        EVE_cmd_number_burst(coordinates::speed::x, coordinates::speed::y, fonts::custom, 0, cal.returnVar(CAL::DATA_ECU::VehicleSpeed));
        // End KPH Display

        // Start Display Engine Temp
        // Side Bar (math should be (int)12*(temp/maxtemp))
        for (int i = 0; i < (cal.returnVar(CAL::DATA_ECU::CoolantTemp)*12)/engineData::maxTemperature; i++) // MAX i = 12 (with given box dimensions)
        {
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            
            if(i < 6) EVE_color_rgb_burst(coolantColors[0]);
            else if(i > 9) EVE_color_rgb_burst(color::Red);
            else EVE_color_rgb_burst(coolantColors[i-5]);

            EVE_cmd_dl_burst(VERTEX2II((coordinates::coolantBar::x-coordinates::coolantBar::width),
                ((coordinates::coolantBar::y-coordinates::coolantBar::height)
                - (i*(coordinates::coolantBar::height+coordinates::coolantBar::vertical_gap))),
                0, 0));

            EVE_cmd_dl_burst(VERTEX2II(coordinates::coolantBar::x,(coordinates::coolantBar::y
                - (i*(coordinates::coolantBar::height+coordinates::coolantBar::vertical_gap))), 0, 0));
            EVE_cmd_dl_burst(DL_END);
        }

        // End Sidebar
        EVE_color_rgb_burst(color::White);
        EVE_cmd_romfont(fonts::custom, fonts::large);
        EVE_cmd_text_burst(coordinates::coolantText::x, coordinates::coolantText::y, fonts::regularLarge, 0, "TEMP");
        EVE_cmd_number_burst(coordinates::coolant::x, coordinates::coolant::y, fonts::custom, EVE_OPT_RIGHTX, cal.returnVar(CAL::DATA_ECU::CoolantTemp));
        // End Display Engine Temp

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