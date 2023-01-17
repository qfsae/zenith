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
        break;
    }
}

void varToBuf(const CAL::data &data, CAL::CAN_msg_t &msg, int &var){
    switch (data.dataType)
    {
    case CAL::DataType::uint8:
        msg.data[data.start_idx] = (var/data.multiplier);
        break;
    case CAL::DataType::Float:
        msg.data[data.start_idx] = (var/data.multiplier);
        break;
    case CAL::DataType::int16:
        msg.data[data.start_idx - 1] = ((int)(var/data.multiplier) >> 8);
        msg.data[data.start_idx] = ((int)(var/data.multiplier));
        break;
    case CAL::DataType::boolean:
        if(var == 1) msg.data[data.start_idx] |= data.bitmask;
        if(var == 0) msg.data[data.start_idx] &= ~data.bitmask;
        break;
    case CAL::DataType::statusField:
        //Not Supported
        break;
    default:
        break;
    }
}

void CAL::CAL::updateVar(const data &var, int value){
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

int bufToVar(const CAL::data &data, CAL::CAN_msg_t &msg){
    if(data.dataType == CAL::DataType::boolean){
        return ((msg.data[data.start_idx] & data.bitmask) > 0);
    }
    else{
        return ((int)msg.data[data.start_idx] & data.bitmask) * data.multiplier;
    }
}

int CAL::CAL::returnVar(const data &CANdata, int &data){
    switch (CANdata.id)
    {
    case MOTEC_ID::ECU_1:
        return bufToVar(CANdata, ecu1);
        break;
    case MOTEC_ID::ECU_2:
        return bufToVar(CANdata, ecu2);
        break;
    case MOTEC_ID::PDM_1:
        return bufToVar(CANdata, pdm1);
        break;
    case MOTEC_ID::PDM_2:
        return bufToVar(CANdata, pdm2);
    case CAN_ID::DASH:
        return bufToVar(CANdata, dash);
    default:
        break;
    }
}