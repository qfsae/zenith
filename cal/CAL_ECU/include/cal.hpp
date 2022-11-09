#pragma once

#include <stdint.h>

typedef struct
{
  uint32_t id;        /* 29 bit identifier                               */
  uint8_t  data[8];   /* Data field                                      */
  uint8_t  len;       /* Length of data field in bytes                   */
  uint8_t  ch;        /* Object channel(Not use)                         */
  uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
  uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg_t;

namespace CAL
{
    // MOTEC CAN ID's
    namespace MOTEC_ID
    {
        static constexpr uint32_t ECU_1 = 0x118;
        static constexpr uint32_t ECU_2 = 0x119;
    } // namespace MOTEC_ID

    /**
     * @brief Runs Proper Update Procedures Based on CAN ID
     * 
     * @param can_recv CAN Message Recieved
     */
    //void parse(CAN_msg_t can_recv);

    class ECU
    {
        public:

            ECU();

            /**
             * @brief Update ECU Container Values
             * 
             * @param can_recv CAN Message Recieved
             * @returns 0 if no id match
             */
            int update(CAN_msg_t &can_recv);

            // Engine RPM (rpm/100)
            uint8_t eSpeed;
            // Throttle Body Position (%)
            uint8_t tPos;
            // Vehicle Speed (km/h)
            uint8_t vSpeed;
            // Coolant Temperature (°C)
            uint8_t cTemp;
            // Engine Temperature (°C)
            uint8_t eTemp;
            // Fuel Temperature (°C)
            uint8_t fTemp;
            // Transmission Temperature (°C)
            uint16_t tTemp;
            // Differential Temperature (°C)
            uint8_t dTemp;
            // Fuel Pressure (10 delta-kPa)
            uint8_t fPressure;
            // Steering Wheel Angle (0.1°)
            uint16_t SteeringAngle;

            // Engine State (True == RUN)
            bool eState;
            // Warning Source (True == Warning)
            bool warning;
            // Brake State (True == ON)
            bool bState;
            // Gear Neutral Switch (True == ON)
            bool neutral;
            // Clutch Switch (True == ON)
            bool cSwitch;
            // Clutch State (True = Disengaged)
            bool cState;
            // Driver Pit Switch (True == ON)
            bool dpSwitch;
            // Engine Run Switch (True == ON)
            bool eSwitch;
            // Driver Switch 1 (True == ON)
            bool d1Switch;
            // Driver Switch 2 (True == ON)
            bool d2Switch;

            // Driver Rotary Switch 1 (-1 = 255)
            uint8_t d1Rotary;
            // Driver Rotary Switch 2 (-1 = 255)
            uint8_t d2Rotary;

    };
    
} // namespace CAL
