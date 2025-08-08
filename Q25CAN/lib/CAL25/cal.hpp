// cal.hpp
#ifndef CAL_HPP
#define CAL_HPP

#include "data_structs.hpp"
#include "inverter.hpp"

namespace CAL {

/**
 * @brief CAN Abstraction Layer (only inverter implemented)
 */
class CAL {
public:
    CAL();

    Inverter inverter;

    /**
     * @brief Store an incoming CAN frame in the correct buffer.
     * @param CAN_msg  The raw frame to store.
     * @return 0 if the ID was recognized, 1 otherwise.
     */
    int updatePackage(CAN_msg_t &CAN_msg);

    /**
     * @brief Encode a value into the transmit buffer.
     */
    void updateVar(const data &d, int   value);
    void updateVar(const data &d, float value);
    void updateVar(const data &d, bool  value);

    /**
     * @brief Decode a value out of the last‑received frame.
     */
    int    returnVar(const data &d);
    int    returnVar(const data &d, int    &out);
    int    returnVar(const data &d, uint8_t &out);
    int    returnVar(const data &d, float  &out);
    int    returnVar(const data &d, bool   &out);

    /**
     * @brief Get a reference to the packed CAN_msg_t for sending.
     */
    CAN_msg_t &package(uint32_t id);
    CAN_msg_t &package(const data &d);

private:
    CAN_msg_t inverter_data{};
    CAN_msg_t fallback_data{};
};

// ─────────────────────────────────────────────────────────────────────────────
// Packing/unpacking helpers
// ─────────────────────────────────────────────────────────────────────────────

void varToBuf(CAN_msg_t &msg, const data &d, int   var);
void varToBuf(CAN_msg_t &msg, const data &d, float var);
void varToBuf(CAN_msg_t &msg, const data &d, bool  var);

void bufToVar(const CAN_msg_t &msg, const data &d, int    &out);
void bufToVar(const CAN_msg_t &msg, const data &d, float  &out);
void bufToVar(const CAN_msg_t &msg, const data &d, bool   &out);
void bufToVar(const CAN_msg_t &msg, const data &d, uint8_t &out);

} // namespace CAL

#endif // CAL_HPP