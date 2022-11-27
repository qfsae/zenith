/*
@file    tft.c / tft.cpp
@brief   TFT handling functions for Steering Display Firmware
@version 1.0
@date    
@author  
 */

#include "EVE.h"
#include "tft_data.h"
#include "tft.h"

#define TEST_UTF8 0


// Memory-map defines
#define MEM_LOGO 0x00000000 // Start-address of logo, needs 19378 bytes of memory

uint8_t tft_active = 0;
uint16_t num_profile_a, num_profile_b;

#define LAYOUT_Y1 66

void touch_calibrate(void) {
#if defined (EVE_NHD_42)
    EVE_memWrite32(REG_TOUCH_TRANSFORM_A, 0x00038460);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_B, 0xFFFC730D);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_C, 0xFFFC61E6);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_D, 0xFFFFC781);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_E, 0x0001C539);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_F, 0xFFE99EAF);
#endif

/* activate this if you are using a module for the first time or if you need to re-calibrate it */
/* write down the numbers on the screen and either place them in one of the pre-defined blocks above or make a new block */
#if 0
    /* calibrate touch and displays values to screen */
    EVE_cmd_dl(CMD_DLSTART);
    EVE_cmd_dl(DL_CLEAR_RGB | BLACK);
    EVE_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_text((EVE_HSIZE/2), 50, 26, EVE_OPT_CENTER, "Please tap on the dot.");
    EVE_cmd_calibrate();
    EVE_cmd_dl(DL_DISPLAY);
    EVE_cmd_dl(CMD_SWAP);
    EVE_execute_cmd();

    uint32_t touch_a, touch_b, touch_c, touch_d, touch_e, touch_f;

    touch_a = EVE_memRead32(REG_TOUCH_TRANSFORM_A);
    touch_b = EVE_memRead32(REG_TOUCH_TRANSFORM_B);
    touch_c = EVE_memRead32(REG_TOUCH_TRANSFORM_C);
    touch_d = EVE_memRead32(REG_TOUCH_TRANSFORM_D);
    touch_e = EVE_memRead32(REG_TOUCH_TRANSFORM_E);
    touch_f = EVE_memRead32(REG_TOUCH_TRANSFORM_F);

    EVE_cmd_dl(CMD_DLSTART);
    EVE_cmd_dl(DL_CLEAR_RGB | BLACK);
    EVE_cmd_dl(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG);
    EVE_cmd_dl(TAG(0));

    EVE_cmd_text(5, 15, 26, 0, "TOUCH_TRANSFORM_A:");
    EVE_cmd_text(5, 30, 26, 0, "TOUCH_TRANSFORM_B:");
    EVE_cmd_text(5, 45, 26, 0, "TOUCH_TRANSFORM_C:");
    EVE_cmd_text(5, 60, 26, 0, "TOUCH_TRANSFORM_D:");
    EVE_cmd_text(5, 75, 26, 0, "TOUCH_TRANSFORM_E:");
    EVE_cmd_text(5, 90, 26, 0, "TOUCH_TRANSFORM_F:");

    EVE_cmd_setbase(16L);
    EVE_cmd_number(310, 15, 26, EVE_OPT_RIGHTX|8, touch_a);
    EVE_cmd_number(310, 30, 26, EVE_OPT_RIGHTX|8, touch_b);
    EVE_cmd_number(310, 45, 26, EVE_OPT_RIGHTX|8, touch_c);
    EVE_cmd_number(310, 60, 26, EVE_OPT_RIGHTX|8, touch_d);
    EVE_cmd_number(310, 75, 26, EVE_OPT_RIGHTX|8, touch_e);
    EVE_cmd_number(310, 90, 26, EVE_OPT_RIGHTX|8, touch_f);

    EVE_cmd_dl(DL_DISPLAY); /* instruct the co-processor to show the list */
    EVE_cmd_dl(CMD_SWAP); /* make this list active */
    EVE_execute_cmd();

    while(1);
#endif
}

/**
 * @brief Initialize the TFT
 * 
 */
void TFT_init(void) {
    if(E_OK == EVE_init())
    {
        tft_active = 1;

        EVE_memWrite8(REG_PWM_DUTY, 0x10);  // Set up backlight, range is from 0 = off to 0x80 = max
        EVE_memWrite16(REG_PWM_HZ, 0x2710); // Set backlight hz to maximum (10000hz)

        touch_calibrate();

        /* LOAD IMAGES INTO MEMORY HERE */
        EVE_cmd_inflate(MEM_LOGO, logo, sizeof(logo)); // Load logo into gfx-memory and de-compress it
        /* LOAD IMAGES INTO MEMORY HERE */
    }
}


uint16_t toggle_state = 0;
uint16_t display_list_size = 0;

/**
 * @brief Check for touch events and setup vars for TFT_display()
 * 
 */
void TFT_touch(void) {
    uint8_t tag;

    if(tft_active != 0) {
        if(EVE_IS_BUSY == EVE_busy()) { // Is EVE still processing the last display list?
            return;
        }

        display_list_size = EVE_memRead16(REG_CMD_DL); // Debug-information, get the size of the last generated display-list

        tag = EVE_memRead8(REG_TOUCH_TAG); // read the value for the first touch point
    }
}

/**
 * @brief Main display update, meant to be called every 20ms or more 
 * Note that this is not the *only* function to update the display - make more as needed for different layouts and call them in loop() ! 
 */
void TFT_display(tft_callback f) {
    if(tft_active != 0) {
        if(EVE_IS_BUSY == EVE_busy()) { // Is EVE still processing the last display list?
            return;
        }

        #if defined (EVE_DMA)
            uint16_t cmd_fifo_size;
            cmd_fifo_size = EVE_dma_buffer_index*4; // Wthout DMA there is no way to tell how many bytes are written to the cmd-fifo
        #endif

        EVE_start_cmd_burst();          // Start writing to the cmd-fifo as one stream of bytes, only sending the address once
        EVE_cmd_dl_burst(CMD_DLSTART);  // Start the display list
        EVE_cmd_dl_burst(DL_CLEAR_RGB | // Set the default clear color to white
                        WHITE);         
        EVE_cmd_dl_burst(DL_CLEAR |     // Clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers
                        CLR_COL | 
                        CLR_STN | 
                        CLR_TAG);  
        EVE_cmd_dl_burst(TAG(0));

        /* DYNAMIC DISPLAY CODE HERE */
        f(); // call the callback of user specified UI
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

        #if defined (EVE_DMA)
            uint16_t cmd_fifo_size;
            cmd_fifo_size = EVE_dma_buffer_index*4; /* without DMA there is no way to tell how many bytes are written to the cmd-fifo */
        #endif

        EVE_start_cmd_burst();
        EVE_cmd_dl_burst(CMD_DLSTART);
        EVE_cmd_dl_burst(DL_CLEAR_RGB | RED);
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