/**
 * @file cal.cpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.5
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cal.hpp"

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, int *data){
    if(msg.id == CANdata.id){
        if(CANdata.dataType == DataType::uint8){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::int16){
            *data = ((((int16_t)msg.data[CANdata.byte2] << 8) | msg.data[CANdata.byte1]) & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::boolean){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask) > 0;
        }
        return 0;
    }
    else{
        return 1;
    }
}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, float *data){
    if(msg.id == CANdata.id){
        if(CANdata.dataType == DataType::uint8){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::int16){
            *data = ((((int16_t)msg.data[CANdata.byte2] << 8) | msg.data[CANdata.byte1]) & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::boolean){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask) > 0;
        }
        return 0;
    }
    else{
        return 1;
    }
}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, bool *data){
    if(msg.id == CANdata.id){
        if(CANdata.dataType == DataType::uint8){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::int16){
            *data = ((((int16_t)msg.data[CANdata.byte2] << 8) | msg.data[CANdata.byte1]) & CANdata.bitmask)*CANdata.multiplier;
        }
        else if(CANdata.dataType == DataType::boolean){
            *data = (msg.data[CANdata.byte1] & CANdata.bitmask) > 0;
        }
        return 0;
    }
    else{
        return 1;
    }
}