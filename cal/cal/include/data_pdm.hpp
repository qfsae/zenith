#pragma once
/**
 * @file data_pdm.hpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief Encases CAL::DATA_PDM
 * @version 0.1
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "data_structs.hpp"

namespace CAL
{
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
