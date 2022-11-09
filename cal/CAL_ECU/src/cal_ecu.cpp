#include "cal.hpp"

using namespace CAL;

ECU::ECU(){
    this->eSpeed = 0;
    this->tPos = 0;
    this->vSpeed = 0;
    this->cTemp = 0;
    this->fTemp = 0;
    this->tTemp = 0;
    this->dTemp = 0;
    this->fPressure = 0;
    this->SteeringAngle = 0;
    this->eState = false;
    this->bState = false;
    this->neutral = true;
    this->cSwitch = true;
    this->cState = true;
    this->dpSwitch = false;
    this->eSwitch = false;
    this->d1Switch = false;
    this->d2Switch = false;
    this->d1Rotary = 0;
    this->d2Rotary = 0;
}

int ECU::update(CAN_msg_t &can_recv){
    // Switch Possible ECU Broadcast ID's
    switch (can_recv.id)
    {
    case MOTEC_ID::ECU_1: // ECU Data Set 0x118
        this->eSpeed = can_recv.data[0];
        this->tPos = can_recv.data[1];
        this->vSpeed = can_recv.data[2];
        this->cTemp = can_recv.data[3];
        this->eTemp = can_recv.data[4];
        this->fTemp = can_recv.data[5];
        this->tTemp = can_recv.data[6];
        this->dTemp = can_recv.data[7];
        break;
    case MOTEC_ID::ECU_2: // ECU Data Set 0x119
        this->fPressure = can_recv.data[0];
        this->SteeringAngle = ((uint16_t)can_recv.data[2] << 8) | can_recv.data[1];

        break;
    default:
        return 0;
        break;
    }
    return 1;
}