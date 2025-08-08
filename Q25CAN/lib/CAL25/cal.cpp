// cal.cpp
#include "cal.hpp"
#include <Arduino.h>

namespace CAL {

CAL::CAL() = default;

/**
 * @brief Store a new incoming CAN frame, decode its field‐ID + value,
 *        then dispatch to the appropriate Inverter setter.
 * @param CAN_msg  The raw frame from the bus.
 * @return 0 if ID==INVERTER (and dispatch attempted), 1 otherwise.
 */
int CAL::updatePackage(CAN_msg_t &CAN_msg) {
    // Copy so we can safely modify/peek
    CAN_msg_t temp = CAN_msg;

    // Check if the ID is recognized
    switch (CAN_msg.id) {
        case CAN_ID::INVERTER:
            inverter_data = temp;
            return inverter.handleMessage(temp);

        // UNIMPLEMENTED
        //   case CAN_ID::UNIMPLEMENTED:
        //     return UNIMPLEMENTED.incomingFrame(temp);

      default:
        // Unknown ID
        Serial.print("Error: Unknown ID received: ");
        Serial.println(CAN_msg.id);
        return 1;
    }
}

// USED with updateVar
CAN_msg_t &CAL::package(uint32_t id) {
    switch (id) {
      case CAN_ID::INVERTER:
        return inverter_data;
    //   case CAN_ID::UNIMPLEMENTED:
    //     return unimplemented_device_data;
      default:
        return fallback_data;
    }
}

CAN_msg_t &CAL::package(const data &d) {
    return package(d.id);
}





void CAL::updateVar(const data &d, int value) {
    CAN_msg_t &pkg = package(d.id);
    pkg.id  = d.id;
    pkg.len = d.len;
    varToBuf(pkg, d, value);
}

void CAL::updateVar(const data &d, float value) {
    CAN_msg_t &pkg = package(d.id);
    pkg.id  = d.id;
    pkg.len = d.len;
    varToBuf(pkg, d, value);
}

void CAL::updateVar(const data &d, bool value) {
    CAN_msg_t &pkg = package(d.id);
    pkg.id  = d.id;
    pkg.len = d.len;
    varToBuf(pkg, d, value);
}

int CAL::returnVar(const data &d) {
    int temp = 0;
    bufToVar(package(d.id), d, temp);
    return temp;
}

int CAL::returnVar(const data &d, int &out) {
    bufToVar(package(d.id), d, out);
    return 0;
}

int CAL::returnVar(const data &d, uint8_t &out) {
    bufToVar(package(d.id), d, out);
    return 0;
}

int CAL::returnVar(const data &d, float &out) {
    bufToVar(package(d.id), d, out);
    return 0;
}

int CAL::returnVar(const data &d, bool &out) {
    bufToVar(package(d.id), d, out);
    return 0;
}

// Packing helpers
void varToBuf(CAN_msg_t &msg, const data &d, int var) {
    switch (d.dataType) {
      case DataType::uint8:
        msg.data[d.start_idx] = static_cast<uint8_t>(var / d.multiplier);
        break;
      case DataType::int16: {
        int v = var / d.multiplier;
        msg.data[d.start_idx]     = static_cast<uint8_t>( v       & 0xFF);
        msg.data[d.start_idx + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        break;
      }
      case DataType::Float:
        msg.data[d.start_idx] = static_cast<uint8_t>(static_cast<float>(var) / d.multiplier);
        break;
      case DataType::boolean:
        if (var)
          msg.data[d.start_idx] |= d.bitmask;
        else
          msg.data[d.start_idx] &= ~d.bitmask;
        break;
      case DataType::statusField:
        // not supported
        break;
    }
}

void varToBuf(CAN_msg_t &msg, const data &d, float var) {
    switch (d.dataType) {
      case DataType::uint8:
        msg.data[d.start_idx] = static_cast<uint8_t>(static_cast<int>(var / d.multiplier));
        break;
      case DataType::int16: {
        int v = static_cast<int>(var / d.multiplier);
        msg.data[d.start_idx]     = static_cast<uint8_t>( v       & 0xFF);
        msg.data[d.start_idx + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        break;
      }
      case DataType::Float:
        msg.data[d.start_idx] = static_cast<uint8_t>(var / d.multiplier);
        break;
      case DataType::boolean:
        if (var != 0.0f)
          msg.data[d.start_idx] |= d.bitmask;
        else
          msg.data[d.start_idx] &= ~d.bitmask;
        break;
      case DataType::statusField:
        // not supported
        break;
    }
}

void varToBuf(CAN_msg_t &msg, const data &d, bool var) {
    switch (d.dataType) {
      case DataType::uint8:
        msg.data[d.start_idx] = static_cast<uint8_t>((var ? 1 : 0) / d.multiplier);
        break;
      case DataType::int16: {
        int v = (var ? 1 : 0) / d.multiplier;
        msg.data[d.start_idx]     = static_cast<uint8_t>( v       & 0xFF);
        msg.data[d.start_idx + 1] = static_cast<uint8_t>((v >> 8) & 0xFF);
        break;
      }
      case DataType::Float:
        msg.data[d.start_idx] = static_cast<uint8_t>((var ? 1.0f : 0.0f) / d.multiplier);
        break;
      case DataType::boolean:
        if (var)
          msg.data[d.start_idx] |= d.bitmask;
        else
          msg.data[d.start_idx] &= ~d.bitmask;
        break;
      case DataType::statusField:
        // not supported
        break;
    }
}

// Unpacking helpers
void bufToVar(const CAN_msg_t &msg, const data &d, int &out) {
    switch (d.dataType) {
      case DataType::uint8:
        out = (msg.data[d.start_idx] & d.bitmask) * d.multiplier;
        break;
      case DataType::int16: {
        uint16_t raw = static_cast<uint16_t>(msg.data[d.start_idx])
                     | (static_cast<uint16_t>(msg.data[d.start_idx + 1]) << 8);
        out = (raw & d.bitmask) * d.multiplier;
        break;
      }
      case DataType::Float:
        out = static_cast<int>((msg.data[d.start_idx] & d.bitmask) * d.multiplier);
        break;
      case DataType::boolean:
        out = ((msg.data[d.start_idx] & d.bitmask) != 0) ? 1 : 0;
        break;
      case DataType::statusField:
        out = 0;
        break;
    }
}

void bufToVar(const CAN_msg_t &msg, const data &d, float &out) {
    switch (d.dataType) {
      case DataType::uint8:
        out = (msg.data[d.start_idx] & d.bitmask) * d.multiplier;
        break;
      case DataType::int16: {
        uint16_t raw = static_cast<uint16_t>(msg.data[d.start_idx])
                     | (static_cast<uint16_t>(msg.data[d.start_idx + 1]) << 8);
        out = (raw & d.bitmask) * d.multiplier;
        break;
      }
      case DataType::Float:
        out = (msg.data[d.start_idx] & d.bitmask) * d.multiplier;
        break;
      case DataType::boolean:
        out = ((msg.data[d.start_idx] & d.bitmask) != 0) ? 1.0f : 0.0f;
        break;
      case DataType::statusField:
        out = 0.0f;
        break;
    }
}

void bufToVar(const CAN_msg_t &msg, const data &d, bool &out) {
    out = ((msg.data[d.start_idx] & d.bitmask) != 0);
}

void bufToVar(const CAN_msg_t &msg, const data &d, uint8_t &out) {
    switch (d.dataType) {
      case DataType::uint8:
        out = (msg.data[d.start_idx] & d.bitmask);
        break;
      case DataType::int16:
        out = ((msg.data[d.start_idx] | (msg.data[d.start_idx + 1] << 8)) & d.bitmask);
        break;
      case DataType::Float:
        out = static_cast<uint8_t>((msg.data[d.start_idx] & d.bitmask) * d.multiplier);
        break;
      case DataType::boolean:
        out = ((msg.data[d.start_idx] & d.bitmask) != 0) ? 1 : 0;
        break;
      case DataType::statusField:
        out = 0;
        break;
    }
}

} // namespace CAL