#ifndef CAL_H
#define CAL_H

#include <cstdint>

struct CALMessage {
    uint32_t id;
    uint8_t data[];
};

class CAL {
    public:
        virtual bool init() = 0;
        virtual CALMessage receive() = 0;
        virtual bool send(CALMessage) = 0;

        // Called on message receive if the underlying hardware has Rx interrupt
        void setReceiveHandler(void (*handle)()) {
            receiveHandler = handle;
        }

        // Called on transmit complete if underlying hardware has Tx interrupt
        void setSendHandler(void (*handle)()) {
            sendHandler = handle;
        }
        
    private:
        void (*receiveHandler)();
        void (*sendHandler)();
};

#endif