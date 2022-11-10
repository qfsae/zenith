#include "cal.hpp"

using namespace CAL;

ECUPacket::ECUPacket(DATA_ECU){
//TODO: add switch case for all DATA_ECU cases
}

int ECUPacket::update(CAN_msg_t &can_recv){
    if(can_recv.id == ID){
        dataLast = can_recv.data[frame] & bitmask;
    }
    else{
        return 0;
    }
}

int ECUPacket::get(){
    return dataLast;
}