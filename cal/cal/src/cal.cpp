/**
 * @file CAL: cal.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 
 * @version 4
 * @date 2023-01-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "cal.hpp"

CAL::CAL::CAL(){
    // Zero Initialize All Values
    ecu1 = { MOTEC_ID::ECU_1, {0, 0, 0, 0, 0, 0, 0, 0}, 8, 0, 0, 0 };
    ecu2 = { MOTEC_ID::ECU_2, {0, 0, 0, 0, 0, 0, 0, 0}, 8, 0, 0, 0 };
    pdm1 = { MOTEC_ID::PDM_1, {0, 0, 0, 0, 0, 0, 0, 0}, 8, 0, 0, 0 };
    pdm2 = { MOTEC_ID::PDM_2, {0, 0, 0, 0, 0, 0, 0, 0}, 8, 0, 0, 0 };
    dash = { CAN_ID::DASH, {0, 0}, 2, 0, 0, 0 };
};

CAL::CAN_msg_t &CAL::CAL::package(uint32_t id){
    switch (id)
    {
    case MOTEC_ID::ECU_1:
        return ecu1;
        break;
    case MOTEC_ID::ECU_2:
        return ecu2;
        break;
    case MOTEC_ID::PDM_1:
        return pdm1;
        break;
    case MOTEC_ID::PDM_2:
        return pdm2;
        break;
    case CAN_ID::DASH:
        return dash;
    default:
        break;
    }
}

CAL::CAN_msg_t &CAL::CAL::package(const data &var){
    switch (var.id)
    {
    case MOTEC_ID::ECU_1:
        return ecu1;
        break;
    case MOTEC_ID::ECU_2:
        return ecu2;
        break;
    case MOTEC_ID::PDM_1:
        return pdm1;
        break;
    case MOTEC_ID::PDM_2:
        return pdm2;
        break;
    case CAN_ID::DASH:
        return dash;
        break;
    default:
        break;
    }
}

int CAL::CAL::updatePackage(CAN_msg_t &CAN_msg){
    switch (CAN_msg.id)
    {
    case MOTEC_ID::ECU_1:
        ecu1 = CAN_msg;
        break;
    case MOTEC_ID::ECU_2:
        ecu2 = CAN_msg;
        break;
    case MOTEC_ID::PDM_1:
        pdm1 = CAN_msg;
        break;
    case MOTEC_ID::PDM_2:
        pdm2 = CAN_msg;
        break;
    case CAN_ID::DASH:
        dash = CAN_msg;
        break;
    default:
        return 1;
        break;
    }
    return 0;
}

void varToBuf(CAL::CAN_msg_t &msg, const CAL::data &CANdata, int &var){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        msg.data[CANdata.start_idx] = (uint8_t)(var/CANdata.multiplier);
        break;
    case CAL::DataType::Float:
        msg.data[CANdata.start_idx] = (float)(var/CANdata.multiplier);
        break;
    case CAL::DataType::int16:
        msg.data[CANdata.start_idx - 1] = ((int)(var/CANdata.multiplier) >> 8);
        msg.data[CANdata.start_idx] = ((int)(var/CANdata.multiplier));
        break;
    case CAL::DataType::boolean:
        if(var == 1) msg.data[CANdata.start_idx] |= CANdata.bitmask;
        if(var == 0) msg.data[CANdata.start_idx] &= ~CANdata.bitmask;
        break;
    case CAL::DataType::statusField:
        //Not Supported
        break;
    default:
        break;
    }
}

void varToBuf(CAL::CAN_msg_t &msg, const CAL::data &CANdata, bool &var){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        msg.data[CANdata.start_idx] = (uint8_t)(var/CANdata.multiplier);
        break;
    case CAL::DataType::Float:
        msg.data[CANdata.start_idx] = (float)(var/CANdata.multiplier);
        break;
    case CAL::DataType::int16:
        msg.data[CANdata.start_idx - 1] = ((int)(var/CANdata.multiplier) >> 8);
        msg.data[CANdata.start_idx] = ((int)(var/CANdata.multiplier));
        break;
    case CAL::DataType::boolean:
        if(var == true) msg.data[CANdata.start_idx] |= CANdata.bitmask;
        if(var == false) msg.data[CANdata.start_idx] &= ~CANdata.bitmask;
        break;
    case CAL::DataType::statusField:
        //Not Supported
        break;
    default:
        break;
    }
}

void varToBuf(CAL::CAN_msg_t &msg, const CAL::data &CANdata, float &var){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        msg.data[CANdata.start_idx] = (int)(var/CANdata.multiplier);
        break;
    case CAL::DataType::Float:
        msg.data[CANdata.start_idx] = (float)(var/CANdata.multiplier);
        break;
    case CAL::DataType::int16:
        msg.data[CANdata.start_idx - 1] = ((int)(var/CANdata.multiplier) >> 8);
        msg.data[CANdata.start_idx] = ((int)(var/CANdata.multiplier));
        break;
    case CAL::DataType::boolean:
        if(var == 1) msg.data[CANdata.start_idx] |= CANdata.bitmask;
        if(var == 0) msg.data[CANdata.start_idx] &= ~CANdata.bitmask;
        break;
    case CAL::DataType::statusField:
        //Not Supported
        break;
    default:
        break;
    }
}

void CAL::CAL::updateVar(const data &CANdata, int value){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        varToBuf(ecu1, CANdata, value);
        break;
    case MOTEC_ID::ECU_2:
        varToBuf(ecu2, CANdata, value);
        break;
    case MOTEC_ID::PDM_1:
        varToBuf(pdm1, CANdata, value);
        break;
    case MOTEC_ID::PDM_2:
        varToBuf(pdm2, CANdata, value);
        break;
    case CAN_ID::DASH:
        varToBuf(dash, CANdata, value);
    default:
        break;
    }
}


void CAL::CAL::updateVar(const data &CANdata, float value){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        varToBuf(ecu1, CANdata, value);
        break;
    case MOTEC_ID::ECU_2:
        varToBuf(ecu2, CANdata, value);
        break;
    case MOTEC_ID::PDM_1:
        varToBuf(pdm1, CANdata, value);
        break;
    case MOTEC_ID::PDM_2:
        varToBuf(pdm2, CANdata, value);
        break;
    case CAN_ID::DASH:
        varToBuf(dash, CANdata, value);
    default:
        break;
    }
}

void CAL::CAL::updateVar(const data &CANdata, bool value){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        varToBuf(ecu1, CANdata, value);
        break;
    case MOTEC_ID::ECU_2:
        varToBuf(ecu2, CANdata, value);
        break;
    case MOTEC_ID::PDM_1:
        varToBuf(pdm1, CANdata, value);
        break;
    case MOTEC_ID::PDM_2:
        varToBuf(pdm2, CANdata, value);
        break;
    case CAN_ID::DASH:
        varToBuf(dash, CANdata, value);
    default:
        break;
    }
}

void bufToVar(CAL::CAN_msg_t &msg, const CAL::data &CANdata, int &data){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::int16:
        data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::Float:
        data = ((float)(msg.data[CANdata.start_idx] & CANdata.bitmask))*CANdata.multiplier;
        break;
    case CAL::DataType::boolean:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask) > 0;
        break;
    case CAL::DataType::statusField:
        data = (CAL::StatusField)(msg.data[CANdata.start_idx]);
    default:
        break;
    }
}

void bufToVar(CAL::CAN_msg_t &msg, const CAL::data &CANdata, float &data){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::int16:
        data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::Float:
        data = ((float)(msg.data[CANdata.start_idx] & CANdata.bitmask))*CANdata.multiplier;
        break;
    case CAL::DataType::boolean:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask) > 0;
        break;
    case CAL::DataType::statusField:
        data = (CAL::StatusField)(msg.data[CANdata.start_idx]);
    default:
        break;
    }
}

void bufToVar(CAL::CAN_msg_t &msg, const CAL::data &CANdata, bool &data){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::int16:
        data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::Float:
        data = ((float)(msg.data[CANdata.start_idx] & CANdata.bitmask))*CANdata.multiplier;
        break;
    case CAL::DataType::boolean:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask) > 0;
        break;
    case CAL::DataType::statusField:
        data = (CAL::StatusField)(msg.data[CANdata.start_idx]);
    default:
        break;
    }
}

void bufToVar(CAL::CAN_msg_t &msg, const CAL::data &CANdata, uint8_t &data){
    switch (CANdata.dataType)
    {
    case CAL::DataType::uint8:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::int16:
        data = ((((int16_t)msg.data[CANdata.start_idx + 1] << 8) | msg.data[CANdata.start_idx]) & CANdata.bitmask)*CANdata.multiplier;
        break;
    case CAL::DataType::Float:
        data = ((float)(msg.data[CANdata.start_idx] & CANdata.bitmask))*CANdata.multiplier;
        break;
    case CAL::DataType::boolean:
        data = (msg.data[CANdata.start_idx] & CANdata.bitmask) > 0;
        break;
    case CAL::DataType::statusField:
        data = (CAL::StatusField)(msg.data[CANdata.start_idx]);
    default:
        break;
    }
}

int CAL::CAL::returnVar(const data &CANdata){
    int data;
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::ECU_2:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_2:
        bufToVar(ecu1, CANdata, data);
    case CAN_ID::DASH:
        bufToVar(ecu1, CANdata, data);
    default:
        break;
    }
    return data;
}

int CAL::CAL::returnVar(const data &CANdata, int &data){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::ECU_2:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_2:
        bufToVar(ecu1, CANdata, data);
    case CAN_ID::DASH:
        bufToVar(ecu1, CANdata, data);
    default:
        break;
    }
    return 0;
}

int CAL::CAL::returnVar(const data &CANdata, uint8_t &data){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::ECU_2:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_2:
        bufToVar(ecu1, CANdata, data);
    case CAN_ID::DASH:
        bufToVar(ecu1, CANdata, data);
    default:
        break;
    }
    return 0;
}

int CAL::CAL::returnVar(const data &CANdata, float &data){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::ECU_2:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_2:
        bufToVar(ecu1, CANdata, data);
    case CAN_ID::DASH:
        bufToVar(ecu1, CANdata, data);
    default:
        break;
    }
    return 0;
}

int CAL::CAL::returnVar(const data &CANdata, bool &data){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::ECU_2:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_1:
        bufToVar(ecu1, CANdata, data);
        break;
    case MOTEC_ID::PDM_2:
        bufToVar(ecu1, CANdata, data);
    case CAN_ID::DASH:
        bufToVar(ecu1, CANdata, data);
    default:
        break;
    }
    return 0;
}