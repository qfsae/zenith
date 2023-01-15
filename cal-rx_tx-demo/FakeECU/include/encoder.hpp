#pragma once
/**
 * @file encoder.hpp
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-01-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdint.h>
#include "cal.hpp"

namespace CAL
{
   class Encoder
   {
   private:
    CAN_msg_t ecu1;
    CAN_msg_t ecu2;
    CAN_msg_t pdm1;
    CAN_msg_t pdm2;
    CAN_msg_t dash;
    /**
     * @brief Decoder update method repackaged to return int 
     * 
     * @param data The Data Packet to extract
     * @param msg The CAN_msg to extract the data from
     * @returns Data as an integer
     */
    int bufToVar(const CAL::data &data, CAN_msg_t &msg);
   public:
   /**
    * @brief Construct a new Encoder object - Acts as storage for data
    */
    Encoder();
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
    CAN_msg_t &package(const CAL::data &var);
    /**
     * @brief Update the value of some data currently stored by the Encoder object
     * 
     * @param var The Data object to update
     * @param value The new Value to assign to the Data Object
     */
    void update(const CAL::data &var, int value);
    /**
     * @brief Get the Data currently stored for a data object
     * 
     * @param var The Data object to retrieve
     * @returns The currently stored value
     */
    int getVar(const CAL::data &var);
   };
    
} // namespace CAL
