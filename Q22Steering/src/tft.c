/**
 * @file tft.c
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @author Logan Lim
 * @author Ethan Peterson (portfolio.petetech.net)
 * @brief TFT Display Code
 * @version 1.2
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
// Programmers Manual: https://brtchip.com/wp-content/uploads/Support/Documentation/Programming_Guides/ICs/EVE/FT81X_Series_Programmer_Guide.pdf
/**
 * Programmers Manual:
 * Useful Pages:
 * 240 - loading ROM fonts (larger fonts)
 * 156 - Fonts (sizes and numbering)
 * 22 - drawing shapes and lines
 * 158/9 - Parameter "OPTION"
 * 
 */
#include "EVE.h"
#include "tft_data.h"
#include "tft.h"
#include "errors.h"

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
// Goes from light blue, though shades of yellow, to orange and then red

// Memory-map defines
#define MEM_LOGO 0x00000000 // Start-address of logo
#define CENTER_N 0x0000f000 // start-address of n_center, needs 48672 bytes
#define CENTER_1 0x0001AE20 // start address of center_1, needs 48672 bytes
#define CENTER_2 0x00026C40 // start address of center_2, needs 48672 bytes
#define CENTER_3 0x00032A60 // start address of center_3, needs 48672 bytes
#define CENTER_4 0x0003E880 // start address of center_4, needs 48672 bytes
#define CENTER_5 0x0004A6A0 // start address of center_5, needs 48672 bytes

// Define Custom Font - NEEDS TO BE ASSIGNED BEFORE USE
#define C_FONT 1

// Define RPM Constants
#define RPM_CHORD_X 245
#define RPM_CHORD_Y 44

// Define Gear Constants
#define GEAR_CHORD_X 165
#define GEAR_CHORD_Y 88

// Define Error Constants
#define ERR_FONT 18
#define ERR_CHORD_X 245
#define ERR_CHORD_Y 260

// Define KPH Constants (KPH_KPH refers to the text)
#define KPH_CHORD_X 0
#define KPH_CHORD_Y 100
#define KPH_KPH_FONT 24
#define KPH_KPH_CHORD_X 5
#define KPH_KPH_CHORD_Y (KPH_CHORD_Y + 90)

// Define Coolant Constants
const uint32_t coolantColors[] = {0x00D1FF, 0xe6cc00, 0xe6b400, 0xe69b00, 0xe47200};
#define COOL_W 35
#define COOL_H 18
#define COOL_SCHORD_X 475 // Starting Coordinate (bottom left)
#define COOL_SCHORD_Y 265
#define COOL_V_GAP 4 // Vertical gap between blocks
#define COOL_CHORD_X 430
#define COOL_CHORD_Y KPH_CHORD_Y+20
#define COOL_COOL_CHORD_X 362
#define COOL_COOL_CHORD_Y (COOL_CHORD_Y+70)
#define COOL_COOL_FONT 24

// Engine Constants
#define K_MAX_TEMP 120

/**
 * @brief Initialize the TFT
 * 
 */
uint16_t tft_active = 0;

bool dswitch = false;

const char* TFT_check_err(){
    if(Err_Fuel_Pressure() !=0){
        return Err_Fuel_Pressure();
    }
    if(Err_Battery() !=0){
        return Err_Battery();
    }
    if(Err_Stale_Data() !=0){
        return Err_Stale_Data();
    }
    if(Err_BSPD() !=0){
        return Err_BSPD();
    }
    if(Err_Engine_Temp() !=0){
        return Err_Engine_Temp();
    }
    if(Err_ECU_Warn() !=0){
        return Err_ECU_Warn();
    }
}

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
        EVE_cmd_inflate(CENTER_4, center_4, sizeof(center_4));
        EVE_cmd_inflate(CENTER_5, center_5, sizeof(center_5));
        /* LOAD IMAGES INTO MEMORY HERE */
    }
}

DataHolder ecu_data;
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

    // Start refresh init

        EVE_start_cmd_burst();          // Start writing to the cmd-fifo as one stream of bytes, only sending the address once
        EVE_cmd_dl_burst(CMD_DLSTART);  // Start the display list
        EVE_cmd_dl_burst(DL_COLOR_RGB | BLACK);  // Set the default clear color to black
        EVE_cmd_dl_burst(DL_CLEAR |     // Clear the screen - this and the previous prevent artifacts between list
                        CLR_COL | // Clear the display's bg (background) color
                        CLR_STN | // Clear Stencil
                        CLR_TAG); // Clear tag buffer
        EVE_cmd_dl_burst(TAG(0));

        EVE_cmd_dl_burst(VERTEX_FORMAT(0)); // Tell the display its about to get a bunch of commands

    // End refresh init commands
    // Start Display write

            // Display the gear position
        EVE_cmd_dl_burst(DL_COLOR_RGB | WHITE);
        EVE_cmd_dl_burst(DL_BEGIN | EVE_BITMAPS);
        switch(ecu_data.gear_pos)
        {
            case 0:
                //                  Data address, img format, width, height (of image)
                EVE_cmd_setbitmap_burst(CENTER_N, EVE_ARGB1555, 156, 156); // displays N
                // Position on Screen ( x, y ) ( (0, 0) is top left)
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X, GEAR_CHORD_Y));
                break;
            case 1:
                EVE_cmd_setbitmap_burst(CENTER_1, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X,GEAR_CHORD_Y));
                break;
            case 2:
                EVE_cmd_setbitmap_burst(CENTER_2, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X, GEAR_CHORD_Y));
                break;
            case 3:
                EVE_cmd_setbitmap_burst(CENTER_3, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X, GEAR_CHORD_Y));
                break;
            case 4:
                EVE_cmd_setbitmap_burst(CENTER_4, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X, GEAR_CHORD_Y));
                break;
            case 5:
                EVE_cmd_setbitmap_burst(CENTER_5, EVE_ARGB1555, 156, 156);
                EVE_cmd_dl_burst(VERTEX2F(GEAR_CHORD_X, GEAR_CHORD_Y));
                break;
        }
        EVE_cmd_dl_burst(DL_END);

        // Start RPM Display
        EVE_cmd_romfont(C_FONT, 34);
        EVE_cmd_number_burst(RPM_CHORD_X, RPM_CHORD_Y, C_FONT, EVE_OPT_CENTER, ecu_data.rpm);
        // END RPM Display

        // Start KPH (Kilometers Per Hour) Display
        EVE_cmd_romfont(C_FONT, 34);
        EVE_cmd_text_burst(KPH_KPH_CHORD_X, KPH_KPH_CHORD_Y, KPH_KPH_FONT, 0/*For none*/, "KPH");
        EVE_cmd_number_burst(KPH_CHORD_X, KPH_CHORD_Y, C_FONT, 0, ecu_data.speed);
        // End KPH Display

        // Start Display Engine Temp
        // Side Bar (math should be (int)12*(temp/maxtemp))
        for (int i = 0; i < (ecu_data.coolant_temp*12)/K_MAX_TEMP; i++) // MAX i = 12 (with given box dimensions)
        {
            EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
            if(i < 6) EVE_color_rgb_burst(coolantColors[0]);
            else if(i > 9) EVE_color_rgb_burst(RED);
            else EVE_color_rgb_burst(coolantColors[i-5]);
            EVE_cmd_dl_burst(VERTEX2II((COOL_SCHORD_X-COOL_W), ((COOL_SCHORD_Y-COOL_H) - (i*(COOL_H+COOL_V_GAP))), 0, 0));
            EVE_cmd_dl_burst(VERTEX2II(COOL_SCHORD_X, (COOL_SCHORD_Y - (i*(COOL_H+COOL_V_GAP))), 0, 0));
            EVE_cmd_dl_burst(DL_END);
        }
        // End Sidebar
        EVE_color_rgb_burst(WHITE);
        EVE_cmd_romfont(C_FONT, 33);
        EVE_cmd_text_burst(COOL_COOL_CHORD_X, COOL_COOL_CHORD_Y, COOL_COOL_FONT, 0, "TEMP");
        EVE_cmd_number_burst(COOL_CHORD_X, COOL_CHORD_Y, C_FONT, EVE_OPT_RIGHTX, ecu_data.coolant_temp);
        // End Display Engine Temp

        // Error Write
        if(TFT_check_err()!=0){
            if(!dswitch){
                EVE_cmd_dl_burst(DL_BEGIN | EVE_RECTS);
                EVE_color_rgb_burst(RED);
                EVE_cmd_dl_burst(VERTEX2II(0,250,0,0));
                EVE_cmd_dl_burst(VERTEX2II(500, 300, 0,0));
                EVE_cmd_dl_burst(DL_END);
                EVE_color_rgb_burst(BLACK);
            }
            EVE_cmd_text_burst(ERR_CHORD_X, ERR_CHORD_Y, ERR_FONT, EVE_OPT_CENTER, TFT_check_err());
            EVE_color_rgb_burst(WHITE);
        }
        // End Error Write
    
    // End Display Write



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
