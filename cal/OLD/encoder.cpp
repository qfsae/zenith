/**
 * @file encoder.cpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "encoder.hpp"

using namespace CAL;

Encoder::Encoder(){
    ecu1 = {
        MOTEC_ID::ECU_1,
        {0, 0, 0, 0, 0, 0, 0, 0},
        8,
        0,
        0,
        0
    };
    ecu2 = {
        MOTEC_ID::ECU_2,
        {0, 0, 0, 0, 0, 0, 0, 0},
        8,
        0,
        0,
        0
    };
    pdm1 = {
        MOTEC_ID::PDM_1,
        {0, 0, 0, 0, 0, 0, 0, 0},
        8,
        0,
        0,
        0
    };
    pdm2 = {
        MOTEC_ID::PDM_2,
        {0, 0, 0, 0, 0, 0, 0, 0},
        8,
        0,
        0,
        0
    };
    dash = {
        CAN_ID::DASH,
        {0},
        1,
        0,
        0,
        0
    };
};

CAN_msg_t &Encoder::package(uint32_t id){
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

CAN_msg_t &Encoder::package(const CAL::data &var){
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

void Encoder::varToBuf(const CAL::data &data, CAL::CAN_msg_t &msg, int &var){
    switch (data.dataType)
    {
    case DataType::uint8:
        msg.data[data.start_idx] = (var/data.multiplier);
        break;
    case DataType::Float:
        msg.data[data.start_idx] = (var/data.multiplier);
        break;
    case DataType::int16:
        msg.data[data.start_idx - 1] = ((int)(var/data.multiplier) >> 8);
        msg.data[data.start_idx] = ((int)(var/data.multiplier));
        break;
    case DataType::boolean:
        if(var == 1) msg.data[data.start_idx] |= data.bitmask;
        if(var == 0) msg.data[data.start_idx] &= ~data.bitmask;
        break;
    case DataType::statusField:
        //Not Supported
        break;
    default:
        break;
    }
}

void Encoder::updateVar(const CAL::data &var, int value){
    switch (var.id)
    {
    case MOTEC_ID::ECU_1:
        varToBuf(var, ecu1, value);
        break;
    case MOTEC_ID::ECU_2:
        varToBuf(var, ecu2, value);
        break;
    case MOTEC_ID::PDM_1:
        varToBuf(var, pdm1, value);
        break;
    case MOTEC_ID::PDM_2:
        varToBuf(var, pdm2, value);
        break;
    case CAN_ID::DASH:
        varToBuf(var, dash, value);
    default:
        break;
    }
}

int Encoder::bufToVar(const CAL::data &data, CAN_msg_t &msg){
    if(data.dataType == DataType::boolean){
        return ((msg.data[data.start_idx] & data.bitmask) > 0);
    }
    else{
        return ((int)msg.data[data.start_idx] & data.bitmask) * data.multiplier;
    }
}

int Encoder::returnVar(const CAL::data &var){
    switch (var.id)
    {
    case MOTEC_ID::ECU_1:
        return bufToVar(var, ecu1);
        break;
    case MOTEC_ID::ECU_2:
        return bufToVar(var, ecu2);
        break;
    case MOTEC_ID::PDM_1:
        return bufToVar(var, pdm1);
        break;
    case MOTEC_ID::PDM_2:
        return bufToVar(var, pdm2);
    case CAN_ID::DASH:
        return bufToVar(var, dash);
    default:
        break;
    }
}