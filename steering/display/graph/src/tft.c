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

// Memory-map defines
#define MEM_LOGO 0x00000000 // Start-address of logo, needs 19378 bytes of memory

uint8_t tft_active = 0;
uint16_t num_profile_a, num_profile_b;
typedef struct Point2 {
    int x;
    int y;
} Point2;
Point2 array1[100];
int counter = 0;
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

int get_x(int xpos){
    int now_millis = millis()/1000; // current time
    int pixels_between_x_dividers=70;
    int x_offset=60; // what difference each x divider is separated by
    int graph_width = 300; // in pixels
    int x_axis_endpoint = 400; // in pixel
    return x_axis_endpoint - ((((now_millis - xpos)%graph_width)*pixels_between_x_dividers)/x_offset) /*multiplied by pixels between each divider, divided by actual value between dividers*/;
}
int get_y(int ypos){
    int y_axis_startpoint = 220; //(the y value of (300,0) on graph)
    int y_offset = 3; //what difference each y divider is separated by
    int pixels_between_y_dividers=40;
    return y_axis_startpoint - ((ypos*pixels_between_y_dividers)/y_offset);
}

/**
 * @brief Main display update, meant to be called every 20ms or more 
 * Note that this is not the *only* function to update the display - make more as needed for different layouts and call them in loop() ! 
 */
void TFT_display() {
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
                        BLACK);         
        EVE_cmd_dl_burst(DL_CLEAR |     // Clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers
                        CLR_COL | 
                        CLR_STN | 
                        CLR_TAG);  
        EVE_cmd_dl_burst(TAG(0));

        /* DYNAMIC DISPLAY CODE HERE */
        uint32_t now_millis;
        uint32_t prev_millis = 0;
        now_millis = millis();
        //List that holds where the x and y dividers for the graph are, if changed then other code should also be changed
        int ydividers[3] = {180,140,100};
        int xdividers[4] = {120,190,260,330};
        //Making the outline of the graph, includes title, x and y axis, and labels for x and y axis.
        EVE_cmd_text(160, 19, 29, 0, "Battery Voltage");
        EVE_cmd_text(407, 213, 18, 0, "Time(s)");
        EVE_cmd_text(45, 35, 27, 0, "V");
        EVE_cmd_text(100, 236, 23, 0, "240        180        120          60           0");
        EVE_cmd_text(16, 50, 23, 0, "12");
        EVE_cmd_text(25, 90, 23, 0, "9");
        EVE_cmd_text(25, 130, 23, 0, "6");
        EVE_cmd_text(25, 170, 23, 0, "3");
        EVE_cmd_dl_burst(LINE_WIDTH(24));
        EVE_cmd_dl_burst(DL_COLOR_RGB | RED);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_LINE_STRIP);
        EVE_cmd_dl_burst(VERTEX2II(39, 60,0,0));
        EVE_cmd_dl_burst(VERTEX2II(50, 60,0,0));
        EVE_cmd_dl_burst(VERTEX2II(50, 220,0,0));
        EVE_cmd_dl_burst(VERTEX2II(400, 220,0,0));
        EVE_cmd_dl_burst(VERTEX2II(400, 232,0,0));
        EVE_cmd_dl_burst(DL_END);

        //Plotting all the x and y dividers for the graph (to reference what x,y values are looked at)
        EVE_cmd_dl_burst(DL_BEGIN | EVE_LINES);
        for(int j = 0; j < 3; j++){
            EVE_cmd_dl_burst(VERTEX2II(39, ydividers[j],0,0));
            EVE_cmd_dl_burst(VERTEX2II(50, ydividers[j],0,0));
        }
        for(int k = 0; k < 4; k++){
            EVE_cmd_dl_burst(VERTEX2II(xdividers[k], 220,0,0));
            EVE_cmd_dl_burst(VERTEX2II(xdividers[k], 232,0,0));
        }

        //Plotting the thin white lines for referencing a point on screen
        EVE_cmd_dl_burst(DL_COLOR_RGB | WHITE);
        EVE_cmd_dl_burst(LINE_WIDTH(4));
        for(int j1 = 0; j1 < 3; j1++){
            EVE_cmd_dl_burst(VERTEX2II(52, ydividers[j1],0,0));
            EVE_cmd_dl_burst(VERTEX2II(400, ydividers[j1],0,0));
        }
        for(int k1 = 0; k1 < 4; k1++){
            EVE_cmd_dl_burst(VERTEX2II(xdividers[k1], 60,0,0));
            EVE_cmd_dl_burst(VERTEX2II(xdividers[k1], 219,0,0));
        }

        //This marks the beginning of the plotting of the graph itself
        EVE_cmd_dl_burst(DL_COLOR_RGB | YELLOW);
        EVE_cmd_dl_burst(LINE_WIDTH(16));
        EVE_cmd_dl_burst(DL_END);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_LINE_STRIP);

        //Adjusting the value of the number after the % sign controls how quickly the screen should refresh.
        if(counter%61 == 0){
            //Recording the current time in variable prev_millis
            prev_millis = now_millis;
            
            //Pushing every element in the array one step ahead, this removes the last element in the list (queue implementation).
            for(int j2 = 99; j2 > 0; j2--){
                array1[j2].x = array1[j2-1].x;
                array1[j2].y = array1[j2-1].y;
            }
            
            //Making a new array point with time (x value) determined by prev_millis and assigning a random y value
            array1[0].x = prev_millis/1000;
            array1[0].y = counter%12;
        }
        ++counter;
        
        //Plotting each array point using the get_x and get_y functions
        for(int i = 0; i < 100; i++){
            EVE_cmd_dl_burst(VERTEX2II(get_x(array1[i].x), get_y(array1[i].y),0,0));
        }
            
        EVE_cmd_dl_burst(DL_END);
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

