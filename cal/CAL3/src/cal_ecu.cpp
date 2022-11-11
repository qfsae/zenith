#include "cal.hpp"

using namespace CAL;

ECUPacket::ECUPacket(DATA_ECU data){
    dataLast = 0;
    bitdepth = 8;
    frame2 = 0;

    switch (data)
    {
    case EngineRPM:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 0;
        break;
    case ThrottlePosition:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 1;
        break;
    case VehicleSpeed:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 2;
        break;
    case CoolantTemp:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 3;
        break;
    case EngineTemp:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 4;
        break;
    case FuelTemp:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 5;
        break;
    case TransmissionTemp:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 6;
        break;
    case DifferentialTemp:
        ID = MOTEC_ID::ECU_1;
        bitmask = 0xff;
        frame = 7;
        break;
    case FuelPressure:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0xff;
        frame = 0;
        break;
    case SteeringAngle:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0xff;
        bitdepth = 16;
        frame = 2;
        frame2 = 3;
        break;
    case EngineState:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x80;
        frame = 4;
        break;
    case WarningSource:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x40;
        frame = 4;
        break;
    case BrakeState:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x20;
        frame = 4;
        break;
    case GearNeutralSwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x10;
        frame = 4;
        break;
    case ClutchSwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x08;
        frame = 4;
        break;
    case ClutchState:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x04;
        frame = 4;
        break;
    case DriverPitSwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x02;
        frame = 4;
        break;
    case EngineRunSwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x01;
        frame = 4;
        break;
    case Driver1Switch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x80;
        frame = 5;
        break;
    case Driver2Switch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0x40;
        frame = 5;
        break;
    case Driver1RotarySwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0xff;
        frame = 6;
        break;
    case Driver2RotarySwitch:
        ID = MOTEC_ID::ECU_2;
        bitmask = 0xff;
        frame = 7;
        break;
    default:
        ID = 0x00;
        bitmask = 0xFF;
        dataLast = 0;
        break;
    }
}

int ECUPacket::update(CAN_msg_t &can_recv){
    if(can_recv.id == ID){
        if(bitdepth == 8){
            dataLast = can_recv.data[frame] & bitmask;
        }
        if(bitdepth == 16){
            dataLast = ((int16_t)can_recv.data[frame2] << 8) | can_recv.data[frame];
        }
    }
    else{
        return 0;
    }
}

int ECUPacket::get(){
    return dataLast;
}