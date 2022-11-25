/**
 * @file cal.cpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.9
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "cal.hpp"

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, uint8_t *data){

    // Return 1 on incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on incorrect Data Type
    //if(!((CANdata.dataType == DataType::int16) || (CANdata.dataType == DataType::uint8))) return 2;

    if(CANdata.dataType == DataType::uint8){
        *data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }
    else if(CANdata.dataType == DataType::int16){
        *data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }

    return 2;

}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, int8_t *data){

    // Return 1 on incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on incorrect Data Type
    //if(!((CANdata.dataType == DataType::int16) || (CANdata.dataType == DataType::uint8))) return 2;

    if(CANdata.dataType == DataType::uint8){
        *data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }
    else if(CANdata.dataType == DataType::int16){
        *data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }

    return 2;

}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, int *data){

    // Return 1 on incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on incorrect Data Type
    //if(!((CANdata.dataType == DataType::int16) || (CANdata.dataType == DataType::uint8))) return 2;

    if(CANdata.dataType == DataType::uint8){
        *data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }
    else if(CANdata.dataType == DataType::int16){
        *data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        return 0;
    }

    return 2;

}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, float *data){

    // Return 1 on Incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on Incorrect Data Type
    if(!(CANdata.dataType == DataType::Float)) return 2;

    *data = (float)((msg.data[CANdata.start_idx] & CANdata.bitmask))*CANdata.multiplier;

    return 0;

}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, bool *data){

    // Return 1 on Incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on Incorrect Data Type
    if(!(CANdata.dataType == DataType::boolean)) return 2;

    *data = (msg.data[CANdata.start_idx] & CANdata.bitmask) > 0;

    return 0;
}

int CAL::update(CAN_msg_t &msg, const CAL::data &CANdata, StatusField *data){
    
    // Return 1 on Incorrect CAN ID
    if(!(msg.id == CANdata.id)) return 1;

    // Return 2 on Incorrect Data Type
    if(!(CANdata.dataType == DataType::statusField)) return 2;

    *data = (StatusField)(msg.data[CANdata.start_idx]);
    
    return 0;

}