#include "cal.hpp"

using namespace CAL;

int ECU::update(CAN_msg_t can_recv){
    // Switch Possible ECU Broadcast ID's
    switch (can_recv.id)
    {
    case MOTEC_ID::ECU_1:
        this->eSpeed = can_recv.data[0];
        this->tPos = can_recv.data[1];
        this->vSpeed = can_recv.data[2];
        this->cTemp = can_recv.data[3];
        this->eTemp = can_recv.data[4];
        this->fTemp = can_recv.data[5];
        this->tTemp = can_recv.data[6];
        this->dTemp = can_recv.data[7];
        break;
    case MOTEC_ID::ECU_2:
        this->fPressure = can_recv.data[0];
        this->SteeringAngle = ((uint16_t)can_recv.data[2] << 8) | can_recv.data[1];

        break;
    default:
        return 0;
        break;
    }
    return 1;
}