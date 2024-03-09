#include "interfaces/interface_inverter.h"
#include "stdio.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "taskHandlers.h"

#define LEFT_INVERTER_ID 10
#define RIGHT_INVERTER_ID 11

typedef enum Motor {
    LEFT,
    RIGHT
} MOTOR;

void os_inverter_setup(void);

void enable_motors(void) {
    disable_motors(); // send out disable command to release lockout
    can_msg_t msg;
    msg.id = LEFT_INVERTER_ID;
    msg.len = 8;
    msg.data[5] = 1;
    can_send_msg(CAN1,&msg,portMAX_DELAY);
    msg.id = RIGHT_INVERTER_ID;
    can_send_msg(CAN1,&msg,portMAX_DELAY);
}

void disable_motors(void) {
    can_msg_t msg;
    msg.id = LEFT_INVERTER_ID;
    msg.len = 8;
    msg.data[5] = 0;
    can_send_msg(CAN1,&msg,portMAX_DELAY);
    msg.id = RIGHT_INVERTER_ID;
    can_send_msg(CAN1,&msg,portMAX_DELAY);
}