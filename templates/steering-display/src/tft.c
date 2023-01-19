/*
@file    tft.c / tft.cpp
@brief   TFT handling functions for Steering Display Firmware
@version 1.0
@date    
@author  
 */

#include "EVE.h"
#include "tft_data.h"

#define TEST_UTF8 0

// Some pre-defined colours
#define RED     0xff0000UL
#define ORANGE  0xffa500UL
#define GREEN   0x00ff00UL
#define BLUE    0x0000ffUL
#define BLUE_1  0x5dade2L
#define YELLOW  0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE  0x800080UL
#define WHITE   0xffffffUL
#define BLACK   0x000000UL
#define TEST_COLOUR 0xA798FF

// Memory-map defines
#define MEM_LOGO 0x00000000 // Start-address of logo
#define CENTER_N 0x0000f000 // start-address of n_center, needs 48672 bytes
#define CENTER_1 0x0001AE20 // start address of center_1, needs 48672 bytes
#define CENTER_2 0x00026C40 // start address of center_2, needs 48672 bytes
#define CENTER_3 0x00032A60 // start address of center_3, needs 48672 bytes

/**
 * @brief Initialize the TFT
 * 
 */
uint16_t tft_active = 0;
void TFT_init(void) {
    if(E_OK == EVE_init())
    {
        tft_active = 1;

        EVE_memWrite8(REG_PWM_DUTY, 0x10);  // Set up backlight, range is from 0 = off to 0x80 = max
        EVE_memWrite16(REG_PWM_HZ, 0x2710); // Set backlight hz to maximum (10000hz)

        /* LOAD IMAGES INTO MEMORY HERE */
        EVE_cmd_inflate(MEM_LOGO, logo, sizeof(logo)); // Load logo into gfx-memory and de-compress it
        EVE_cmd_inflate(CENTER_N, center_n, sizeof(center_n)); // Load logo into gfx-memory and de-compress it
        EVE_cmd_inflate(CENTER_1, center_1, sizeof(center_1));
        EVE_cmd_inflate(CENTER_2, center_2, sizeof(center_2));
        EVE_cmd_inflate(CENTER_3, center_3, sizeof(center_3));
        /* LOAD IMAGES INTO MEMORY HERE */
    }
}


uint16_t display_list_size = 0;
uint16_t gear_position;
/**
 * @brief Main TFT display
 */
void TFT_display(void) {
    if(tft_active != 0) {
        if(EVE_IS_BUSY == EVE_busy()) { // Is EVE still processing the last display list?
            display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */
            return;
        }

        EVE_start_cmd_burst();          // Start writing to the cmd-fifo as one stream of bytes, only sending the address once
        EVE_cmd_dl_burst(CMD_DLSTART);  // Start the display list
        EVE_cmd_dl_burst(DL_COLOR_RGB | // Set the default clear color to white
                        WHITE);         
        EVE_cmd_dl_burst(DL_CLEAR |     // Clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers
                        CLR_COL | 
                        CLR_STN | 
                        CLR_TAG);  
        EVE_cmd_dl_burst(TAG(0));

        EVE_cmd_dl_burst(VERTEX_FORMAT(0));



// //draw right side lines
        EVE_color_rgb_burst(YELLOW); // TODO: refactor
        EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        //horizontal lines
        EVE_cmd_dl_burst(VERTEX2II(0, 0, 0, 0));;
        EVE_cmd_dl_burst(VERTEX2II(10, 272, 0, 0));

        EVE_cmd_dl_burst(VERTEX2II(0, 272, 0, 0));
        EVE_cmd_dl_burst(VERTEX2II(480, 252, 0, 0));

        EVE_cmd_dl_burst(VERTEX2II(480, 272, 0, 0));
        EVE_cmd_dl_burst(VERTEX2II(470, 0, 0, 0));

        EVE_cmd_dl_burst(VERTEX2II(480, 0, 0, 0));
        EVE_cmd_dl_burst(VERTEX2II(0, 20, 0, 0));
        EVE_cmd_dl_burst(DL_END);


        EVE_color_rgb_burst(WHITE); // TODO: refactor
        EVE_cmd_text_burst(240, 40, 28, EVE_OPT_CENTER, "Gauge View");

        // Speed
        EVE_cmd_text_burst(100, 60, 27, EVE_OPT_CENTER, "Speed");
        EVE_cmd_gauge(100, 120, 48, EVE_OPT_NOPOINTER, 10, 8, 0, 100);
        EVE_color_rgb_burst(RED); // TODO: refactor
        EVE_cmd_gauge(100, 120, 48, EVE_OPT_NOBACK | EVE_OPT_NOTICKS, 10, 8, 44, 100);

        // RPM
        EVE_color_rgb_burst(WHITE); 
        EVE_cmd_text_burst(380, 60, 27,EVE_OPT_CENTER, "RPM");
        EVE_cmd_gauge(380, 120, 48,EVE_OPT_NOPOINTER, 10, 8, 0, 100);
        EVE_color_rgb_burst(RED); // TODO: refactor
        EVE_cmd_gauge(380, 120, 48, EVE_OPT_NOBACK | EVE_OPT_NOTICKS, 10, 8, 44, 100);

        // Temp
        EVE_color_rgb_burst(WHITE); 
        EVE_cmd_text_burst(240, 130, 27, EVE_OPT_CENTER, "Temp");
        EVE_cmd_gauge(240, 190, 48,  EVE_OPT_NOPOINTER, 10, 8, 0, 100);
        EVE_color_rgb_burst(RED); // TODO: refactor
        EVE_cmd_gauge(240, 190, 48, EVE_OPT_NOBACK | EVE_OPT_NOTICKS, 10, 8, 44, 100);



        // // // Display the gear position
        // EVE_cmd_dl_burst(DL_COLOR_RGB | MAGENTA);
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);

        // // draw left side lines
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
        // // horizontal lines
        // EVE_cmd_dl_burst(VERTEX2II(20, 90, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(120, 90, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(20, 180, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(120, 180, 0, 0));
        // // vertical lines
        // EVE_cmd_dl_burst(VERTEX2II(20, 0, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(20, 272, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(120, 0, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(120, 272, 0, 0));
        // EVE_cmd_dl_burst(DL_END);

        // // //draw right side lines
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
        // //horizontal lines
        // EVE_cmd_dl_burst(VERTEX2II(360, 90, 0, 0));;
        // EVE_cmd_dl_burst(VERTEX2II(460, 90, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(360, 180, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(460, 180, 0, 0));
        // //vertical lines
        // EVE_cmd_dl_burst(VERTEX2II(360, 0, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(360, 272, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(460, 0, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(460, 272, 0, 0));
        // EVE_cmd_dl_burst(DL_END);

        // //draw RPM
        // EVE_cmd_text_burst(70, 20, 28, EVE_OPT_CENTER, "RPM");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(TEST_COLOUR);
        // EVE_cmd_dl_burst(VERTEX2II(30, 45, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(110, 75, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_number_burst(70, 60, 25, EVE_OPT_CENTER, 10);

        // // draw 2nd box
        // EVE_cmd_text_burst(70, 110, 28, EVE_OPT_CENTER, "???");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(TEST_COLOUR);
        // EVE_cmd_dl_burst(VERTEX2II(30, 135, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(110, 165, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_number_burst(70, 150, 25, EVE_OPT_CENTER, 4);

        // // draw Speed box
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_text_burst(70, 200, 28, EVE_OPT_CENTER, "Speed");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(TEST_COLOUR);
        // EVE_cmd_dl_burst(VERTEX2II(30, 225, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(110, 255, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_number_burst(70, 240, 25, EVE_OPT_CENTER, 4);

        // // draw temp box
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_text_burst(410, 20, 28, EVE_OPT_CENTER, "Temp");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(TEST_COLOUR);
        // EVE_cmd_dl_burst(VERTEX2II(370, 45, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(450, 75, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_number_burst(410, 60, 25, EVE_OPT_CENTER, 20);

        // // draw 5th box
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_text_burst(410, 110, 28, EVE_OPT_CENTER, "???");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(TEST_COLOUR);
        // EVE_cmd_dl_burst(VERTEX2II(370, 135, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(450, 165, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_number_burst(410, 150, 25, EVE_OPT_CENTER, 4);

        // // draw status box
        // EVE_color_rgb_burst(WHITE);
        // EVE_cmd_text_burst(410, 200, 28, EVE_OPT_CENTER, "Status");
        // EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
        // EVE_color_rgb_burst(GREEN);
        // EVE_cmd_dl_burst(VERTEX2II(370, 225, 0, 0));
        // EVE_cmd_dl_burst(VERTEX2II(450, 255, 0, 0));
        // EVE_cmd_dl_burst(DL_END);
        // EVE_color_rgb_burst(WHITE);


        /* DYNAMIC DISPLAY CODE HERE */
        
        EVE_cmd_dl_burst(DL_DISPLAY); // Instruct the co-processor to show the list 
        EVE_cmd_dl_burst(CMD_SWAP); // Make this list active

        EVE_end_cmd_burst(); // Stop writing to the cmd-fifo, the cmd-FIFO will be executed automatically after this or when DMA is done
    }
}


/**
 * @brief display TFT splash screen with Queen's Racing Logo 
 * 
 */
void TFT_splash() {
    if(tft_active != 0) {
        while (EVE_busy());

        display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */

        EVE_start_cmd_burst();
        EVE_cmd_dl_burst(CMD_DLSTART);
        EVE_cmd_dl_burst(DL_COLOR_RGB | RED);
        EVE_cmd_dl_burst(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
        EVE_cmd_dl_burst(TAG(0));

        EVE_cmd_dl_burst(VERTEX_FORMAT(0));

        // Display the logo
        EVE_cmd_dl_burst(DL_COLOR_RGB | WHITE);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        EVE_cmd_setbitmap_burst(MEM_LOGO, EVE_ARGB1555, 294, 103);
        EVE_cmd_dl_burst(VERTEX2F(93, 84));
        EVE_cmd_dl_burst(DL_END);

        EVE_cmd_dl_burst(DL_DISPLAY); /* instruct the co-processor to show the list */
        EVE_cmd_dl_burst(CMD_SWAP); /* make this list active */

        EVE_end_cmd_burst(); /* stop writing to the cmd-fifo, the cmd-FIFO will be executed automatically after this or when DMA is done */
    }
}
