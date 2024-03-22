/**
 * @file state_rtd.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief Ready to Drive System State
 * @version 0.1
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "sys_state.h"


void vState_RTD(enum SYS_STATE *state){
    if(*state != SYS_STATE_RTD) return;

    // this state is currently unused.
    *state = SYS_STATE_IDLE;
    return;
}

