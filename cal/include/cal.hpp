#pragma once

/**
 * @file cal.hpp
 * @author Jacob Chisholm (jchisholm204)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.2
 * @date 2022-11-11
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

    /**
     * @brief CAN Data Packet Structure for unsigned 8 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte The data's position within the CAN data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte;
        const int bitmask;
        uint8_t data;
    } data_uint8;

    /**
     * @brief CAN Data Packet Structure for signed 8 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte The data's position within the CAN data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself -> This can be updated with the update method
     * 
     */
    /*
    typedef struct
    {
        const int32_t id;
        const int byte;
        const int bitmask;
        uint8_t data;
    } data_int8;*/
    

    /**
     * @brief CAN Data Packet Structure for unsigned 16 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte1 The data's position within the CAN data
     * @param byte2 The position of the second half of the data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte1;
        const int byte2;
        const int bitmask;
        uint16_t data;
    } data_uint16;

    /**
     * @brief CAN Data Packet Structure for 16 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte1 The data's position within the CAN data
     * @param byte2 The position of the second half of the data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte1;
        const int byte2;
        const int bitmask;
        int16_t data;
    } data_int16;


    /**
     * @brief CAN Data Packet Structure for boolean data extracted from uint8_t
     * @param id The CAN ID that the Data is found in
     * @param byte The data's position within the CAN data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte;
        const int bitmask;
        bool data;
    }data_bool;
    

    /**
     * @brief CAN Data Packet Structure for 16 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte1 The data's position within the CAN data
     * @param byte2 The position of the second half of the data
     * @param bitmask The Data's Bitmask
     * @param multiplier The Data's Multiplier
     * @param data The Data Itself (stored as float) -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte;
        const int bitmask;
        const float multiplier;
        float data;
    } data_float8;

    /**
     * @brief CAN Data Packet Structure for 16 bit integers
     * @param id The CAN ID that the Data is found in
     * @param byte1 The data's position within the CAN data
     * @param byte2 The position of the second half of the data
     * @param bitmask The Data's Bitmask
     * @param data The Data Itself (stored as float) -> This can be updated with the update method
     * 
     */
    typedef struct
    {
        const uint32_t id;
        const int byte1;
        const int byte2;
        const int bitmask;
        const float multiplier;
        float data;
    } data_float16;

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
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_uint8 *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    //int update(CAN_msg_t &msg, data_int8 *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_uint16 *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_int16 *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_bool *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_float8 *data);

    /**
     * @brief CAN Data Update Method
     * 
     * @param msg The CAN Message to update the data from
     * @param data The Data Packet to update
     * 
     * @returns 0 Success || 1 Incorrect ID
     */
    extern int update(CAN_msg_t &msg, data_float16 *data);


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
        extern data_uint8 EngineRPM;

        /**
         * @brief Throttle Position(%): 
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 1 | Mask 0xFF
         */
        extern data_uint8 ThrottlePosition;

        /**
         * @brief Vehicle Speed(km/h): 
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 2 | Mask 0xFF
         */
        extern data_uint8 VehicleSpeed;

        /**
         * @brief Engine Coolant Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 3 | Mask 0xFF
         */
        extern data_uint8 CoolantTemp;

        /**
         * @brief Engine Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 4 | Mask 0xFF
         */
        extern data_uint8 EngineTemp;

        /**
         * @brief Fuel Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 5 | Mask 0xFF
         */
        extern data_uint8 FuelTemp;

        /**
         * @brief Transmission Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 6 | Mask 0xFF
         */
        extern data_uint8 TransmissionTemp;

        /**
         * @brief Differential Transmission Temperature(°C):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x118 | Byte 7 | Mask 0xFF
         */
        extern data_uint8 DifferentialTemp;

        /**
         * @brief Fuel Pressure(10 delta-kPa):
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x119 | Byte 0 | Mask 0xFF
         */
        extern data_uint8 FuelPressure;

        /**
         * @brief Steering Wheel Angle(0.1°):
         * Contained as signed 16-bit Integer,
         * Accessed at: ID 0x119 | Byte 2+3 | Mask 0xFF
         */
        extern data_int16 SteeringAngle;

        /**
         * @brief Engine State(True == RUN):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x80
         */
        extern data_bool EngineState;

        /**
         * @brief Warning Source(True == Warning):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x40
         */
        extern data_bool WarningSource;

        /**
         * @brief Brake State(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x20
         */
        extern data_bool BrakeState;

        /**
         * @brief Gear Neutral Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x10
         */
        extern data_bool GearNeutralSwitch;

        /**
         * @brief Clutch Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x08
         */
        extern data_bool ClutchSwitch;

        /**
         * @brief Clutch State(True == Disengaged):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x04
         */
        extern data_bool ClutchState;

        /**
         * @brief Driver Pit Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x02
         */
        extern data_bool DriverPitSwitch;

        /**
         * @brief Engine Run Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 4 | Mask 0x01
         */
        extern data_bool EngineRunSwitch;

        /**
         * @brief Driver 1 Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 5 | Mask 0x80
         */
        extern data_bool Driver1Switch;

        /**
         * @brief Driver 2 Switch(True == ON):
         * Contained as a boolean,
         * Accessed at: ID 0x119 | Byte 5 | Mask 0x40
         */
        extern data_bool Driver2Switch;

        /**
         * @brief Driver 1 Rotary Switch:
         * Contained as unsigned 8-bit integer,
         * Accessed at: ID 0x119 | Byte 6 | Mask 0xFF
         */
        extern data_uint8 Driver1RotarySwitch;

        /**
         * @brief Driver 2 Rotary Switch:
         * Contained as unsigned 8-bit Integer,
         * Accessed at: ID 0x119 | Byte 7 | Mask 0xFF
         */
        extern data_uint8 Driver2RotarySwitch;

    } // namespace DATA_ECU

    namespace DATA_PDM
    {
        
    } // namespace DATA_PDM
    
    
} // namespace CAL
