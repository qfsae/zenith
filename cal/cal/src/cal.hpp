#pragma once
/**
 * @file cal.hpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief CAL (CAN Abstraction Layer)
 * @version 3.8
 * @date 2022-11-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "data_structs.hpp"
#include "data_dash.hpp"
#include "data_ecu.hpp"
#include "data_pdm.hpp"

// Encase entire library within CAL namespace
namespace CAL {

    /**
     * @brief CAL Object
     * Used for storing CAN bus Data
     */
    class CAL
    {
    private:
        CAN_msg_t ecu1;
        CAN_msg_t ecu2;
        CAN_msg_t pdm1;
        CAN_msg_t pdm2;
        CAN_msg_t dash;
    public:
    /**
        * @brief Construct a new CAL object - Acts as storage for data
        */
        CAL();
        /**
         * @brief Return the CAN_msg currently stored using an identifier
         * 
         * @param id The Identifier of the CAN_msg to return 
         * @returns reference to internal CAN_msg object
         */
        CAN_msg_t &package(uint32_t id);

        /**
         * @brief Return the CAN_msg stored using a data stored within it 
         * 
         * @param var The child of the CAN_msg to return
         * @returns Reference to parent CAN_msg of var
         */
        CAN_msg_t &package(const data &var);

        /**
         * @brief Update an entire package based off of a CAN message
         * 
         * @param CAN_msg The CAN_msg to update from
         * @returns 0 Success | 1 Incorrect ID
         */
        int updatePackage(CAN_msg_t &CAN_msg);

        /**
         * @brief Update the value of some data currently stored by the CAL object
         * 
         * @param var The Data object to update
         * @param value The new Value to assign to the Data Object
         */
        void updateVar(const data &var, int value);
        
        /**
         * @brief Update the value of some data currently stored by the CAL object
         * 
         * @param var The Data object to update
         * @param value The new Value to assign to the Data Object
         */
        void updateVar(const data &var, float value);

        /**
         * @brief Update the value of some data currently stored by the CAL object
         * 
         * @param var The Data object to update
         * @param value The new Value to assign to the Data Object
         */
        void updateVar(const data &var, bool value);

        /**
         * @brief Get the Data currently stored for a data object
         * 
         * @param CANdata The Data object to retrieve
         * @returns Data (ONLY WORKS WITH INTEGERS)
         */
        int returnVar(const data &CANdata);

        /**
         * @brief Get the Data currently stored for a data object
         * 
         * @param CANdata The Data object to retrieve
         * @param data return by reference
         */
        int returnVar(const data &CANdata, int &data);
        
        /**
         * @brief Get the Data currently stored for a data object
         * 
         * @param CANdata The Data object to retrieve
         * @param data return by reference
         */
        int returnVar(const data &CANdata, uint8_t &data);
        
        /**
         * @brief Get the Data currently stored for a data object
         * 
         * @param CANdata The Data object to retrieve
         * @param data return by reference
         */
        int returnVar(const data &CANdata, float &data);
        
         /**
         * @brief Get the Data currently stored for a data object
         * 
         * @param CANdata The Data object to retrieve
         * @param data return by reference
         */
        int returnVar(const data &CANdata, bool &data);    
    };    

} // namespace CAL
