#pragma once
/**
 * @file data_dash.hpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Encases CAL::DATA_DASH
 * @version 0.1
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "data_structs.hpp"


// CAL namespace
namespace CAL{

    /**
     * @brief Encases all of the Data Types coming off of the Dashboards CAN 
     * 
     * Items marked with (*) are returned by the ECU
     */
    namespace DATA_DASH{

        /**
         * @brief Gear Up Shift Register
         * Contained as Boolean Value,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0x80
         */
        constexpr data UpShift = {
            CAN_ID::DASH,
            0,
            0x80,
            1,
            DataType::boolean
        };

        /**
         * @brief Gear Down Shift Register
         * Contained as Boolean Value,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0x40
         */
        constexpr data DownShift = {
            CAN_ID::DASH,
            0,
            0x40,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Switch 1 (*)
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0x20
         */
        constexpr data DriverSwitch1 = {
            CAN_ID::DASH,
            0,
            0x20,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Switch 2
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0x10
         */
        constexpr data DriverSwitch2 = {
            CAN_ID::DASH,
            0,
            0x10,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Pit Switch (*)
         * Contained as a Boolean Value,
         * Accessed at: ID 0x120 | Byte 0 | Mask 0x08
         */
        constexpr data DriverPitSwitch = {
            CAN_ID::DASH,
            0,
            0x08,
            1,
            DataType::boolean
        };

        /**
         * @brief Driver Rotary Switch/Toggle
         * Contained as an unsigned 8 bit integer,
         * Accessed at: ID 0x120 | Byte 1 | Mask 0xFF
         */
        constexpr data DriverRotarySwitch = {
            CAN_ID::DASH,
            1,
            0xFF,
            1,
            DataType::uint8
        };

    } // namespace DATA_DASH
} // namespace CAL