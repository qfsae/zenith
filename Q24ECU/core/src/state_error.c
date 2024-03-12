/**
 * @file state_error.c
 * @author Jacob Chisholm (https://Jchisholm204.github.io)
 * @brief System Run State Code
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "core.h"

void vState_Error(enum SYS_STATE *state){
    if(*state != SYS_STATE_ERR) return;

    // this state is currently unused.
    *state = SYS_STATE_IDLE;
    return;
}

