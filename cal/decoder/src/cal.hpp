#pragma once

/**
 * @file cal.hpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.8
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdint.h>

namespace CAL
{
    typedef struct {
        uint32_t id;        /* 29 bit identifier                               */
        uint8_t  data[8];   /* Data field                                      */
        uint8_t  len;       /* Length of data field in bytes                   */
        uint8_t  ch;        /* Object channel(Not use)                         */
        uint8_t  format;    /* 0 - STANDARD, 1- EXTENDED IDENTIFIER            */
        uint8_t  type;      /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
    } CAN_msg_t;

    // ECU Status Fields
    enum StatusField{
        OutputOff = 0,
        OutputOn = 1,
        OutputFault = 2,
        OutputOverCurrent = 4,
        OutputMaxRetries = 8
    };

    // CAN Data Types
    enum DataType{
        uint8,
        int16,
        boolean,
        Float,
        statusField
    };

    /**
     * @brief CAN Data Packet Structure
     * @param id The CAN ID that the Data is found in
     * @param start_idx Where the data first occurs within the CAN data
     * @param bitmask The Data's Bitmask
     * @param dataType The MoTec Defined Data Type
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int start_idx;
        const int bitmask;
        const float multiplier;
        const DataType dataType;
    } data;

    // MOTEC CAN ID's (ECU/PDM)
    namespace MOTEC_ID
    {
        static constexpr uint32_t ECU_1 = 0x118;
        static constexpr uint32_t ECU_2 = 0x119;
        static constexpr uint32_t PDM_1 = 0x7F0;
        static constexpr uint32_t PDM_2 = 0x7F1;
    } // namespace MOTEC_ID

    // Melanous CAN ID's
    namespace CAN_ID{
        static constexpr uint32_t DASH = 0x120;
    }

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, uint8_t *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, int8_t *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, int *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, float *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, bool *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAL::CAN_msg_t &msg, const CAL::data &CANdata, StatusField *data);

    /**
     * @brief Encases all of the Data Types coming off of the Dashboards CAN 
     * 
     */
    namespace DATA_DASH{

        /**
         * @brief Car Gear 
         * Contained as an unsigned 8 bit integer,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0xFF
         *  0 = Reverse | 1 = Neutral | {2,3,...} = Drive
         */
        constexpr data Gear = {
            CAN_ID::DASH,
            0,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Driver Switch 1 (*)
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 1 | Mask 0x80
         */
        constexpr data DriverSwitch1 = {
            CAN_ID::DASH,
            1,
            0x80,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Switch 2
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 1 | Mask 0x40
         */
        constexpr data DriverSwitch2 = {
            CAN_ID::DASH,
            1,
            0x40,
            1,
            DataType::boolean
        };

        /**
         * @brief Engine Enable Switch (*)
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 1 | Mask 0x20
         */
        constexpr data EngineEnable = {
            CAN_ID::DASH,
            1,
            0x20,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Pit Switch (*)
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 1 | Mask 0x10
         */
        constexpr data DriverPitSwitch = {
            CAN_ID::DASH,
            1,
            0x10,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Rotary Toggle 1
         * Contained as an unsigned 8 bit integer,
         * Accessed at: ID 0x120 | Byte 2 | Mask 0xFF
         */
        constexpr data Driver1RotarySwitch = {
            CAN_ID::DASH,
            2,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Driver Rotary Toggle 2
         * Contained as an unsigned 8 bit integer,
         * Accessed at: ID 0x120 | Byte 3 | Mask 0xFF
         */
        constexpr data Driver2RotarySwitch = {
            CAN_ID::DASH,
            3,
            0xFF,
            1,
            DataType::uint8
        };
        
    } // namespace DATA_DASH

    /**
     * @brief Encases all of the Data Types coming off of the ECU's CAN
     * 
     */
    namespace DATA_ECU{

        /**
         * @brief Engine RPM:
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 0 | Mask 0xFF
         */
        constexpr data EngineRPM = {
            MOTEC_ID::ECU_1,
            0,
            0xFF,
            100,
            DataType::uint8
        };

        /**
         * @brief Throttle Position(%): 
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 1 | Mask 0xFF
         */
        constexpr data ThrottlePosition = {
            MOTEC_ID::ECU_1,
            1,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Vehicle Speed(km/h): 
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 2 | Mask 0xFF
         */
        constexpr data VehicleSpeed = {
            MOTEC_ID::ECU_1,
            2,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Engine Coolant Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 3 | Mask 0xFF
         */
        constexpr data CoolantTemp = {
            MOTEC_ID::ECU_1,
            3,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Engine Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 4 | Mask 0xFF
         */
        constexpr data EngineTemp = {
            MOTEC_ID::ECU_1,
            4,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Fuel Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 5 | Mask 0xFF
         */
        constexpr data FuelTemp = {
            MOTEC_ID::ECU_1,
            5,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Transmission Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 6 | Mask 0xFF
         */
        constexpr data TransmissionTemp = {
            MOTEC_ID::ECU_1,
            6,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Differential Transmission Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 7 | Mask 0xFF
         */
        constexpr data DifferentialTemp = {
            MOTEC_ID::ECU_1,
            7,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Fuel Pressure(10 delta-kPa):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x119 | Byte 0 | Mask 0xFF
         */
        constexpr data FuelPressure = {
            MOTEC_ID::ECU_2,
            0,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Steering Wheel Angle(0.1°):
         * Contained as signed 16-bit Integer,
         * Accessed at: ID 0x119 | Byte 2+3 | Mask 0xFF
         */
        constexpr data SteeringAngle = {
            MOTEC_ID::ECU_2,
            2,
            0xFF,
            1,
            DataType::int16
        };

        /**
         * @brief Engine State(True == RUN):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x80
         */
        constexpr data EngineState = {
            MOTEC_ID::ECU_2,
            4,
            0x80,
            1,
            DataType::boolean
        };

        /**
         * @brief Warning Source(True == Warning):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x40
         */
        constexpr data WarningSource = {
            MOTEC_ID::ECU_2,
            4,
            0x40,
            1,
            DataType::boolean
        };

        /**
         * @brief Brake State(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x20
         */
        constexpr data BrakeState = {
            MOTEC_ID::ECU_2,
            4,
            0x20,
            1,
            DataType::boolean
        };

        /**
         * @brief Gear Neutral Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x10
         */
        constexpr data GearNeutralSwitch = {
            MOTEC_ID::ECU_2,
            4,
            0x10,
            1,
            DataType::boolean
        };

        /**
         * @brief Clutch Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x08
         */
        constexpr data ClutchSwitch = {
            MOTEC_ID::ECU_2,
            4,
            0x08,
            1,
            DataType::boolean
        };

        /**
         * @brief Clutch State(True == Disengaged):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x04
         */
        constexpr data ClutchState = {
            MOTEC_ID::ECU_2,
            4,
            0x04,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Pit Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x02
         */
        constexpr data DriverPitSwitch = {
            MOTEC_ID::ECU_2,
            4,
            0x02,
            1,
            DataType::boolean
        };

        /**
         * @brief Engine Run Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x01
         */
        constexpr data EngineRunSwitch = {
            MOTEC_ID::ECU_2,
            4,
            0x01,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver 1 Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 5 | Mask 0x80
         */
        constexpr data Driver1Switch = {
            MOTEC_ID::ECU_2,
            5,
            0x80,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver 2 Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 5 | Mask 0x40
         */
        constexpr data Driver2Switch = {
            MOTEC_ID::ECU_2,
            5,
            0x40,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver 1 Rotary Switch:
         * Contained as unsigned 8-bit integer,
         * Accessed at: ID 0x119 | Byte 6 | Mask 0xFF
         */
        constexpr data Driver1RotarySwitch = {
            MOTEC_ID::ECU_2,
            6,
            0xFF,
            1,
            DataType::uint8
        };

        /**
         * @brief Driver 2 Rotary Switch:
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x119 | Byte 7 | Mask 0xFF
         */
        constexpr data Driver2RotarySwitch = {
            MOTEC_ID::ECU_2,
            7,
            0xFF,
            1,
            DataType::uint8
        };

    } // namespace DATA_ECU

    /**
     * @brief Encases all of the Data Types coming off of the PDM's CAN
     * 
     */
    namespace DATA_PDM
    {
        /**
         * @brief Throttle Potentiometer Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 0 | Mask 0xFF
         */
        constexpr data ThrottlePotentiometerVoltage = {
            MOTEC_ID::PDM_1,
            0,
            0xFF,
            0.2,
            DataType::Float
        };

        /**
         * @brief Battery Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 1 | Mask 0xFF
         */
        constexpr data BatteryVoltage = {
            MOTEC_ID::PDM_1,
            1,
            0xFF,
            0.1216,
            DataType::Float
        };

        /**
         * @brief Fuel Pump Current:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 2 | Mask 0xFF
         */
        constexpr data FuelPumpCurrent = {
            MOTEC_ID::PDM_1,
            2,
            0xFF,
            0.5,
            DataType::Float
        };

        /**
         * @brief Fuel Pump Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 3 | Mask 0xFF
         */
        constexpr data FuelPumpVoltage = {
            MOTEC_ID::PDM_1,
            3,
            0xFF,
            0.2,
            DataType::Float
        };

        // constexpr data FuelPumpStatus = {
        //     MOTEC_ID::PDM_1,
        //     4,
        //     0xFF,
        //     0.2,
        //     DataType::Float
        // };
        //FAULT FLAG

        /**
         * @brief Kill Switch Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 5 | Mask 0xFF
         */
        constexpr data KillSwitchVoltage = {
            MOTEC_ID::PDM_1,
            5,
            0xFF,
            0.2,
            DataType::Float
        };

        /**
         * @brief Throttle Body Current:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 6 | Mask 0xFF
         */
        constexpr data ThrottleBodyCurrent = {
            MOTEC_ID::PDM_1,
            6,
            0xFF,
            0.2,
            DataType::Float
        };

         /**
         * @brief Throttle Body Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F0 | Byte 7 | Mask 0xFF
         */
        constexpr data ThrottleBodyVoltage = {
            MOTEC_ID::PDM_1,
            7,
            0xFF,
            0.2,
            DataType::Float
        };
        //FAULT FLAGs
        constexpr data tBStatusFault = {
            MOTEC_ID::PDM_2,
            0,
            0xFF,
            1,
            DataType::statusField
        };

        // constexpr data  tBStatusField = {
        //     MOTEC_ID::PDM_2,
        //     1,
        //     0xFF,
        //     ,
        //     DataType::Float
        // };

        // constexpr data cF1Status = {
        //     MOTEC_ID::PDM_2,
        //     2,
        //     0xFF,
        //     ,
        //     DataType::Float
        // };

        // constexpr data cF2Status = {
        //     MOTEC_ID::PDM_2,
        //     3,
        //     0xFF,
        //     ,
        //     DataType::Float
        // };

        /**
         * @brief Fuel Injector Current:
         * Contained as a Float,
         * Accessed at: ID 0x7F1 | Byte 4 | Mask 0xFF
         */
        constexpr data FuelInjectorCurrent = {
            MOTEC_ID::PDM_2,
            4,
            0xFF,
            0.2,
            DataType::Float
        };

        /**
         * @brief Fuel Injector Voltage:
         * Contained as a Float,
         * Accessed at: ID 0x7F1 | Byte 5 | Mask 0xFF
         */
        constexpr data FuelInjectorVoltage = {
            MOTEC_ID::PDM_2,
            5,
            0xFF,
            0.2,
            DataType::Float
        };
        //
        //FAULT FLAG
        // constexpr data fIStatusField = {
        //     MOTEC_ID::PDM_2,
        //     6,
        //     0xFF,
        //     ,
        //     DataType::Float
        // };

        // constexpr data sRStatus = {
        //     MOTEC_ID::PDM_2,
        //     7,
        //     0xFF,
        //     ,
        //     DataType::Float
        // };
    } // namespace DATA_PDM
    
    
} // namespace CAL
