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

#include "sys_state.h"
#include "stdbool.h"
#include "errors.h"
#include "drivers/uart.h"

uint32_t errors[3];
#define get_e(e) (bool)((errors[e/3] >> (e % 32)) & 0x01)
#define set_e(e) ((errors[e/3] |= (uint32_t)(0x1UL << (e % 32))))
#define clear_e(e) ((errors[e/3] &= ~(uint32_t)(0x1UL << (e % 32))))

void vState_Error(enum SYS_STATE *state){
    if(*state == SYS_STATE_INIT){
        errors[0] = 0UL;
        errors[1] = 0UL;
        errors[2] = 0UL;
    }
    if(*state != SYS_STATE_ERR) return;
    uart_write_str(&Serial2, "System has entered into an error state...\n", 10U);
    for(enum SYS_ERROR i = 0; i < SYS_ERROR_MAX; i++){
        if(get_e(i) == true){
            char buf[40];
            snprintf(buf, 40, "Error: %d\n", i);
            uart_write_str(&Serial2, buf, 10);
            clear_e(i);
        }
    }
    uart_write_str(&Serial2, "Returning to idle state\n", 10U);
    // this state is currently unused.
    *state = SYS_STATE_IDLE;
    return;
}

/* BEGIN Error Handling Code */


void controller_throw(enum SYS_ERROR e){
    if(e == SYS_OK) return;
    set_e(e);
}

bool controller_check_error(enum SYS_ERROR e){
    return get_e(e);
}

bool controller_check_fault(void){
    return !(errors[0] == 0 && errors[1] == 0 && errors[2] == 0);
}

/* END Error Handling Code */
