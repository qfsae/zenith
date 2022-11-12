#include "cal.hpp"

using namespace CAL;

/**
 * @brief Engine RPM:
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 0 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::EngineRPM = {
    MOTEC_ID::ECU_1, // ID
    0, // Byte
    0xff, // Bit Mask
    0, // Data Initializer
};

/**
 * @brief Throttle Position(%): 
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 1 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::ThrottlePosition = {
    MOTEC_ID::ECU_1, // ID
    1, // Byte
    0xff, // Bit Mask
    0 // Data Initializer
};

/**
 * @brief Vehicle Speed(km/h): 
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 2 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::VehicleSpeed = {
    MOTEC_ID::ECU_1, // ID
    2, // Byte
    0xff, // Bit Mask
    0, // Data Initializer
};

/**
 * @brief Engine Coolant Temperature(°C):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 3 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::CoolantTemp = {
    MOTEC_ID::ECU_1, //ID
    3, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Engine Temperature(°C):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 4 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::EngineTemp = {
    MOTEC_ID::ECU_1, // ID
    4, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Fuel Temperature(°C):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 5 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::FuelTemp = {
    MOTEC_ID::ECU_1, // ID
    5, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Transmission Temperature(°C):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 6 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::TransmissionTemp = {
    MOTEC_ID::ECU_1, // ID
    6, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Differential Transmission Temperature(°C):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x118 | Byte 7 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::DifferentialTemp = {
    MOTEC_ID::ECU_1, // ID
    7, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Fuel Pressure(10 delta-kPa):
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x119 | Byte 0 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::FuelPressure = {
    MOTEC_ID::ECU_2, // ID
    0, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Steering Wheel Angle(0.1°):
 * Contained as signed 16-bit Integer,
 * Accessed at: ID 0x119 | Byte 2+3 | Mask 0xFF
 */
data_int16 SteeringAngle = {
    MOTEC_ID::ECU_2, // ID
    2, // Byte 1
    3, // Byte 2
    0xff, // Bit Mask
    0
};

/**
 * @brief Engine State(True == RUN):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x80
 */
data_bool EngineState = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x80, // Bit Mask
    0
};

/**
 * @brief Warning Source(True == Warning):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x40
 */
data_bool WarningSource {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x40, // Bit Mask
    0
};

/**
 * @brief Brake State(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x20
 */
data_bool BrakeState = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x20, // Bit Mask
    0
};

/**
 * @brief Gear Neutral Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x10
 */
data_bool GearNeutralSwitch = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x10, // Bit Mask
    0
};

/**
 * @brief Clutch Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x08
 */
data_bool ClutchSwitch = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x08, // Bit Mask
    0
};

/**
 * @brief Clutch State(True == Disengaged):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x04
 */
data_bool ClutchState = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x04, // Bit Mask
    0
};

/**
 * @brief Driver Pit Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x02
 */
data_bool DriverPitSwitch = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x02, // Bit Mask
    0
};

/**
 * @brief Engine Run Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 4 | Mask 0x01
 */
data_bool EngineRunSwitch = {
    MOTEC_ID::ECU_2, // ID
    4, // Byte
    0x01, // Bit Mask
    0
};

/**
 * @brief Driver 1 Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 5 | Mask 0x80
 */
data_bool Driver1Switch = {
    MOTEC_ID::ECU_2, // ID
    5, // Byte
    0x80, // Bit Mask
    0
};

/**
 * @brief Driver 2 Switch(True == ON):
 * Contained as a boolean,
 * Accessed at: ID 0x119 | Byte 5 | Mask 0x40
 */
data_bool Driver2Switch = {
    MOTEC_ID::ECU_2, // ID
    5, // Byte
    0x40, // Bit Mask
    0
};

/**
 * @brief Driver 1 Rotary Switch:
 * Contained as unsigned 8-bit integer,
 * Accessed at: ID 0x119 | Byte 6 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::Driver1RotarySwitch = {
    MOTEC_ID::ECU_2, // ID
    6, // Byte
    0xff, // Bit Mask
    0
};

/**
 * @brief Driver 2 Rotary Switch:
 * Contained as unsigned 8-bit Integer,
 * Accessed at: ID 0x119 | Byte 7 | Mask 0xFF
 */
data_uint8 CAL::DATA_ECU::Driver2RotarySwitch = {
    MOTEC_ID::ECU_2, // ID
    7, // Byte
    0xff, // Bit Mask
    0
};