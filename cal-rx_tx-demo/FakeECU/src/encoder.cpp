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

void Encoder::update(const CAL::data &var, int value){
    switch (var.id)
    {
    case MOTEC_ID::ECU_1:
        ecu1.data[var.start_idx] = (value/var.multiplier);
        break;
    case MOTEC_ID::ECU_2:
        ecu2.data[var.start_idx] = (value/var.multiplier);
        break;
    case MOTEC_ID::PDM_1:
        pdm1.data[var.start_idx] = (value/var.multiplier);
        break;
    case MOTEC_ID::PDM_2:
        pdm2.data[var.start_idx] = (value/var.multiplier);
        break;
    case CAN_ID::DASH:
        dash.data[var.start_idx] = (value/var.multiplier);
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

int Encoder::getVar(const CAL::data &var){
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