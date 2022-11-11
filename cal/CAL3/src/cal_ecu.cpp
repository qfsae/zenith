#include "cal.hpp"

using namespace CAL;

int update(CAN_msg_t &msg, data_uint8 *data){
    if(msg.id == data->id){
        data->data = msg.data[data->byte] & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}

//int update(CAN_msg_t &msg, data_int8 *data){}


int update(CAN_msg_t &msg, data_uint16 *data){
    if(msg.id == data->id){
        data->data = (((uint16_t)msg.data[data->byte2] << 8) | msg.data[data->byte1]) & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}


int update(CAN_msg_t &msg, data_int16 *data){
    if(msg.id == data->id){
        data->data = (((int16_t)msg.data[data->byte2] << 8) | msg.data[data->byte1]) & data->bitmask;
        return 0;
    }
    else{
        return 1;
    }
}


int update(CAN_msg_t &msg, data_bool *data){
    if(msg.id == data->id){
        data->data = (msg.data[data->byte] & data->bitmask) > 0;
        return 0;
    }
    else{
        return 1;
    }
}