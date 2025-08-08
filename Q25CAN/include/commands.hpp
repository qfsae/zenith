// lib/CAL25/commands.hpp
#ifndef CAL25_COMMANDS_HPP
#define CAL25_COMMANDS_HPP

#include "data_structs.hpp"

namespace CAL {
namespace CMD {

// ─── Inverter Heartbeat / Command (ID = INVERTER + 0xC0) ────────────────
/// torque (Nm ×10), bytes 0–1
static const data TORQUE    = {
    CAN_ID::INVERTER + 0xC0u,  // id
    0,                         // start_idx
    2,                         // len
    0xFF,                      // bitmask per byte
    10,                        // multiplier
    DataType::int16            // 16‑bit signed
};

/// speed (RPM), bytes 2–3
static const data SPEED     = {
    CAN_ID::INVERTER + 0xC0u,
    2,
    2,
    0xFF,
    1,
    DataType::int16
};

/// direction bit @ byte4
static const data DIRECTION = {
    CAN_ID::INVERTER + 0xC0u,
    4,
    1,
    0x01,
    1,
    DataType::boolean
};

/// run bit @ byte5
static const data RUN       = {
    CAN_ID::INVERTER + 0xC0u,
    5,
    1,
    0x01,
    1,
    DataType::boolean
};

} // namespace CMD
} // namespace CAL

#endif // CAL25_COMMANDS_HPP