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
#define MEM_LOGO 0x00000000 // Start-address of logo

void Display::displaySplash(){
    if(tft_active != 0) {
        while (EVE_busy());

        //display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */

        EVE_start_cmd_burst();
        EVE_cmd_dl_burst(CMD_DLSTART);
        EVE_cmd_dl_burst(DL_COLOR_RGB | color::Red);
        EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
        EVE_cmd_dl_burst(TAG(0));

        EVE_cmd_dl_burst(VERTEX_FORMAT(0));

        // Display the logo
        EVE_cmd_dl_burst(DL_COLOR_RGB | color::White);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_LOGO, EVE_ARGB1555, 294, 103);
        EVE_cmd_dl_burst(VERTEX2F(93, 84));
        EVE_cmd_dl_burst(DL_END);

        EVE_cmd_dl_burst(DL_DISPLAY); /* instruct the co-processor to show the list */
        EVE_cmd_dl_burst(CMD_SWAP); /* make this list active */

        EVE_end_cmd_burst(); /* stop writing to the cmd-fifo, the cmd-FIFO will be executed automatically after this or when DMA is done */
    }
}