/**
 * @file cal.cpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.2
 * @date 2022-11-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cal.hpp"

int CAL::update(CAN_msg_t &msg, data_uint8 *data){
    if(msg.id == data->id){
        data->data = msg.data[data->byte] & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}

//int update(CAN_msg_t &msg, data_int8 *data){}


int CAL::update(CAN_msg_t &msg, data_uint16 *data){
    if(msg.id == data->id){
        data->data = (((uint16_t)msg.data[data->byte2] << 8) | msg.data[data->byte1]) & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}


int CAL::update(CAN_msg_t &msg, data_int16 *data){
    if(msg.id == data->id){
        data->data = (((int16_t)msg.data[data->byte2] << 8) | msg.data[data->byte1]) & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}


int CAL::update(CAN_msg_t &msg, data_bool *data){
    if(msg.id == data->id){
        data->data = (msg.data[data->byte] & data->bitmask) > 0;
        return 0;
    }
    else{
        return 1;
    }
}

int CAL::update(CAN_msg_t &msg, data_double8 *data){
    if(msg.id == data->id){
        data->data = float(msg.data[data->byte] & data->bitmask) * data->multiplier;
        return 0;
    }
    else{
        return 1;
    }
}

int CAL::update(CAN_msg_t &msg, data_double16 *data){
    if(msg.id == data->id){
        data->data = float((((int16_t)msg.data[data->byte2] << 8) | msg.data[data->byte1]) & data->bitmask) * data->multiplier;
        return 0;
    }
    else{
        return 1;
    }
}