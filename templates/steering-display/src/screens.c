#include "EVE.h"
#include "tft_data.h"
#include "tft.h"

// add functions or EVE calls to this file to run them onscreen
// Put UI specific code here to decouple it from the tft library Logan set up
// Note that we cannot run these in main.cpp due to name mangling.
// Keep all the logic of switching between displays in main.cpp

void display_code() {
    // place display code in here.
    EVE_color_rgb(BLACK);
    EVE_cmd_number(5, 15, 28, 0, 69);
}