#pragma once

/**
 * @file cal.hpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.6
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

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

    enum DataType{
        uint8,
        int16,
        boolean,
        Float
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

    // MOTEC CAN ID's
    namespace MOTEC_ID
    {
        static constexpr uint32_t ECU_1 = 0x118;
        static constexpr uint32_t ECU_2 = 0x119;
    } // namespace MOTEC_ID

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, const CAL::data &CANdata, int *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, const CAL::data &CANdata, float *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param CANdata The Search Data
     * @param data pointer for user defined data
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, const CAL::data &CANdata, bool *data);


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
            1,
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
            0xFF,1,
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

    namespace DATA_PDM
    {
        
    } // namespace DATA_PDM
    
    
} // namespace CAL
