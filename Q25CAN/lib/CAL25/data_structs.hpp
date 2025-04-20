// data_structs.hpp
#ifndef DATA_STRUCTS_HPP
#define DATA_STRUCTS_HPP

#include <stdint.h>

namespace CAL {

/**
 * @brief CAN message structure (8‑byte frame)
 */
typedef struct {
    uint32_t id;        // 29‑bit identifier
    uint8_t  data[8];   // Data payload
    uint8_t  len;       // Number of bytes in data[]
    uint8_t  ch;        // Channel (unused)
    uint8_t  format;    // 0 = STANDARD, 1 = EXTENDED
    uint8_t  type;      // 0 = DATA FRAME, 1 = REMOTE FRAME
} CAN_msg_t;

/**
 * @brief Data type enumeration for signal descriptors
 */
enum class DataType {
    uint8,
    int16,
    Float,
    boolean,
    statusField
};

/**
 * @brief Signal descriptor within a CAN frame
 */
typedef struct {
    uint32_t id;         // Associated CAN ID
    uint8_t  start_idx;  // Byte offset in data[]
    uint8_t  len;        // Number of bytes occupied
    uint8_t  bitmask;    // Mask for the bits in those bytes
    uint8_t  multiplier; // Scaling factor
    DataType dataType;   // Interpretation type
} data;

/**
 * @brief Known CAN IDs (only inverter supported)
 */
namespace CAN_ID {
    static const uint32_t INVERTER = 2147485360u;
}

} // namespace CAL

#endif // DATA_STRUCTS_HPP