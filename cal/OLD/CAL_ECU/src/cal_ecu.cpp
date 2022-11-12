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
    this->gnSwitch = true;
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
        this->eSpeed = can_recv.data[0] & 0xFF;
        this->tPos = can_recv.data[1] & 0xFF;
        this->vSpeed = can_recv.data[2] & 0xFF;
        this->cTemp = can_recv.data[3] & 0xFF;
        this->eTemp = can_recv.data[4] & 0xFF;
        this->fTemp = can_recv.data[5] & 0xFF;
        this->tTemp = can_recv.data[6] & 0xFF;
        this->dTemp = can_recv.data[7] & 0xFF;
        break;
    case MOTEC_ID::ECU_2: // ECU Data Set 0x119
        this->fPressure = can_recv.data[0];
        this->SteeringAngle = ((int16_t)can_recv.data[2] << 8) | can_recv.data[1];
        this->eState = (can_recv.data[4] & 0x80) > 0;
        this->warning = (can_recv.data[4] & 0x40) > 0;
        this->bState = (can_recv.data[4] & 0x20) > 0;
        this->gnSwitch = (can_recv.data[4] & 0x10) > 0;
        this->cSwitch = (can_recv.data[4] & 0x08) > 0;
        this->cState = (can_recv.data[4] & 0x04) > 0;
        this->dpSwitch = (can_recv.data[4] & 0x02) > 0;
        this->eSwitch = (can_recv.data[4] & 0x01) > 0;
        this->d1Switch = (can_recv.data[5] & 0x80) > 0;
        this->d2Switch = (can_recv.data[5] & 0x40) > 0;
        this->d1Rotary = can_recv.data[6];
        this->d2Rotary = can_recv.data[7];
        break;
    default:
        return 0;
        break;
    }
    return 1;
}