// lib/CAL25/inverter.hpp
#ifndef INVERTER_HPP
#define INVERTER_HPP

#include "data_structs.hpp"

namespace CAL {

// forward‐declare the unpack helper
void bufToVar(const CAN_msg_t &msg, const data &d, float &out);

/**
 * @brief Identifiers for inverter sub‑messages.
 */
enum class InverterField : uint8_t {
  MSG_VOLTAGE     = 0x01,
  MSG_CURRENT     = 0x02,
  MSG_TEMPERATURE = 0x03
};

/**
 * @brief Parser/updater for inverter CAN sub‑messages.
 */
class Inverter {
public:
  Inverter() = default;

  /**
   * @brief Decode and apply one sub‑message.
   * @return 1 if handled, 0 otherwise.
   */
  int handleMessage(const CAN_msg_t &msg) {
    lastMsg = msg;

    // need at least sub‑ID + one byte of payload
    if (msg.len < 2) return 0;

    // 1) which field?
    InverterField field = static_cast<InverterField>(msg.data[0]);

    // 2) prepare descriptor for bytes [1..len)
    data desc{};
    desc.id         = msg.id;
    desc.start_idx  = 1;
    desc.len        = msg.len - 1;
    desc.bitmask    = 0xFF;
    desc.multiplier = 1;

    // 3) dispatch per‐field
    switch (field) {
      case InverterField::MSG_VOLTAGE: {
        desc.dataType = DataType::Float;
        float v = 0.0f;
        bufToVar(msg, desc, v);
        voltage = v;
        return 1;
      }
      case InverterField::MSG_CURRENT: {
        desc.dataType = DataType::Float;
        float c = 0.0f;
        bufToVar(msg, desc, c);
        current = c;
        return 1;
      }
      case InverterField::MSG_TEMPERATURE: {
        desc.dataType = DataType::Float;
        float t = 0.0f;
        bufToVar(msg, desc, t);
        temperature = t;
        return 1;
      }
      default:
        return 0;
    }
  }

  // accessors
  float getVoltage()    const { return voltage; }
  float getCurrent()    const { return current; }
  float getTemperature()const { return temperature; }

private:
  CAN_msg_t lastMsg{};
  float     voltage     = 0.0f;
  float     current     = 0.0f;
  float     temperature = 0.0f;
};

} // namespace CAL

#endif // INVERTER_HPP