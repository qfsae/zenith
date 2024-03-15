/**
 * @file state_start.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief High Voltage Power up State (Rules TS Active)
 * @version 0.1
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys_state.h"


void vState_Start(enum SYS_STATE *state){

    // this state is currently unused.
    *state = SYS_STATE_IDLE;
    return;


    if(*state == SYS_STATE_TS_ACTIVE){
        // initialize the inverters
        // initialize the tractive system
    }
    if(*state == SYS_STATE_TEST){
        // only enable the inverters
    }
    else {
        *state = SYS_STATE_ERR;
        return;
    }

    // Wait for precharge
    
    for(;;){
        // check for throttle 0 and brake pressed, start button pressed
    }

}

