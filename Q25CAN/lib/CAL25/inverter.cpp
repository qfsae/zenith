// lib/CAL25/inverter.cpp

#include "inverter.hpp"

namespace CAL {

// Decode one CAN frame
int Inverter::handleMessage(const CAN_msg_t &msg) {
    hasSeenAny = true;
    lastRxMs   = millis();
    // Extract 11-bit identifier
    uint16_t id11 = static_cast<uint16_t>(msg.id & 0x7FF);
    // Compute sub-index relative to base offset
    constexpr uint16_t offset = static_cast<uint16_t>(CAN_ID::INVERTER & 0x7FF);
    if (id11 < offset || id11 > offset + 0x11) return 0;
    uint8_t idx = static_cast<uint8_t>(id11 - offset);

    switch (idx) {
        case 0: // 0x0A0: Temperatures #1 
            if (msg.len >= 8) {
                moduleATemp = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                moduleBTemp = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                moduleCTemp = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                gdbTemp     = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 1: // 0x0A1: Temperatures #2 
            if (msg.len >= 8) {
                controlBoardTemp  = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                rtd1Temp          = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                rtd2Temp          = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                motorHotSpotTemp  = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 2: // 0x0A2: Temperatures #3 & Torque Shudder 
            if (msg.len >= 8) {
                coolantTemp        = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                inverterHotSpotTemp= CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                motorTemp          = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                torqueShudder      = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 3: // 0x0A3: Analog Input Voltages (bits 0-9,10-19,20-29,32-41,42-51,52-61) 
            if (msg.len >= 8) {
                for (uint8_t i = 0; i < 6; ++i) {
                    uint16_t start = (i < 3 ? 10 * i : 32 + 10 * (i - 3));
                    analogInputs[i] = static_cast<float>(CAL::extractBits(msg.data, start, 10)) * 0.01f;
                }
                return 1;
            }
            break;

        case 4: // 0x0A4: Digital Input Status
            if (msg.len >= 1) {
                for (uint8_t i = 0; i < 8; ++i) {
                    digitalInputs[i] = (msg.data[0] >> i) & 0x1;
                }
                return 1;
            }
            break;

        case 5: // 0x0A5: Motor Position Information
            if (msg.len >= 8) {
                motorAngleElectrical     = CAL::toUnsigned(msg.data[0], msg.data[1]);
                motorSpeed               = CAL::toUnsigned(msg.data[2], msg.data[3]);
                electricalOutputFrequency= CAL::toUnsigned(msg.data[4], msg.data[5]);
                deltaResolverFiltered    = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 6: // 0x0A6: Current Information
            if (msg.len >= 8) {
                phaseACurrent = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                phaseBCurrent = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                phaseCCurrent = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                dcBusCurrent  = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 7: // 0x0A7: Voltage Information
            if (msg.len >= 8) {
                dcBusVoltage  = CAL::toUnsigned(msg.data[0], msg.data[1]) * 0.1f;
                outputVoltage = CAL::toUnsigned(msg.data[2], msg.data[3]) * 0.1f;
                vabVdVoltage  = CAL::toUnsigned(msg.data[4], msg.data[5]) * 0.1f;
                vbcVqVoltage  = CAL::toUnsigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 8: // 0x0A8: Flux Information
            if (msg.len >= 8) {
                vd_ff       = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                vq_ff       = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                idFeedback  = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                iqFeedback  = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 9: // 0x0A9: Internal Voltages
            if (msg.len >= 8) {
                refVoltage1_5 = CAL::toUnsigned(msg.data[0], msg.data[1]) * 0.001f;
                refVoltage2_5 = CAL::toUnsigned(msg.data[2], msg.data[3]) * 0.001f;
                refVoltage5_0 = CAL::toUnsigned(msg.data[4], msg.data[5]) * 0.001f;
                refVoltage12_0= CAL::toUnsigned(msg.data[6], msg.data[7]) * 0.001f;
                return 1;
            }
            break;

        case 10: // 0x0AA: Internal States
            if (msg.len >= 6) {
                vsmState                 = msg.data[0];
                pwmFrequency             = msg.data[1];
                inverterState            = msg.data[2];
                relayStates              = msg.data[3];
                inverterRunMode          = (msg.data[4] & 0x1);
                selfSensingAssistEnable  = (msg.data[4] >> 1) & 0x1;
                uint8_t b5 = msg.data[5];
                directionCommand         = (b5 >> 0) & 0x1;
                bmsActive                = (b5 >> 1) & 0x1;
                bmsLimitingMotorTorque   = (b5 >> 2) & 0x1;
                limitMaxSpeed            = (b5 >> 3) & 0x1;
                limitHotSpotInverter     = (b5 >> 4) & 0x1;
                lowSpeedLimiting         = (b5 >> 5) & 0x1;
                limitCoolantDerating     = (b5 >> 6) & 0x1;
                limitStallBurstModel     = (b5 >> 7) & 0x1;
                return 1;
            }
            break;

        case 11: // 0x0AB: Fault Codes
            if (msg.len >= 8) {
                postFaultLo = CAL::toUnsigned(msg.data[0], msg.data[1]);
                postFaultHi = CAL::toUnsigned(msg.data[2], msg.data[3]);
                runFaultLo  = CAL::toUnsigned(msg.data[4], msg.data[5]);
                runFaultHi  = CAL::toUnsigned(msg.data[6], msg.data[7]);
                return 1;
            }
            break;

        case 12: // 0x0AC: Torque & Timer Information
            if (msg.len >= 8) {
                commandedTorque = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                torqueFeedback  = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                powerOnTimer    = static_cast<uint32_t>(msg.data[4])
                                | (static_cast<uint32_t>(msg.data[5]) << 8)
                                | (static_cast<uint32_t>(msg.data[6]) << 16)
                                | (static_cast<uint32_t>(msg.data[7]) << 24);
                return 1;
            }
            break;

        case 13: // 0x0AD: Modulation Index & Flux Weakening
            if (msg.len >= 8) {
                modulationIndex      = CAL::toUnsigned(msg.data[0], msg.data[1]) * 0.01f;
                fluxWeakeningOutput  = CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                idCommandCurrent     = CAL::toSigned(msg.data[4], msg.data[5]) * 0.1f;
                iqCommandCurrent     = CAL::toSigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 14: // 0x0AE: Firmware Information citeturn2file3
            if (msg.len >= 8) {
                eepromVersion   = CAL::toUnsigned(msg.data[0], msg.data[1]);
                softwareVersion = CAL::toUnsigned(msg.data[2], msg.data[3]);
                dateCodeMMDD    = CAL::toUnsigned(msg.data[4], msg.data[5]);
                dateCodeYYYY    = CAL::toUnsigned(msg.data[6], msg.data[7]);
                return 1;
            }
            break;

        case 15: // 0x0AF: Diagnostic Data citeturn2file3
            // Refer to section 6.8 for detailed parsing
            return 1;

        case 16: // 0x0B0: High Speed Message (3ms) citeturn2file3
            if (msg.len >= 8) {
                highSpeedTorqueCommand = CAL::toSigned(msg.data[0], msg.data[1]) * 0.1f;
                highSpeedTorqueFeedback= CAL::toSigned(msg.data[2], msg.data[3]) * 0.1f;
                highSpeedMotorSpeed    = CAL::toUnsigned(msg.data[4], msg.data[5]);
                highSpeedDCBusVoltage  = CAL::toUnsigned(msg.data[6], msg.data[7]) * 0.1f;
                return 1;
            }
            break;

        case 17: // 0x0B1: Torque Capability citeturn2file3
            if (msg.len >= 8) {
                motorTorqueAvailable = CAL::toUnsigned(msg.data[0], msg.data[1]) * 0.1f;
                regenTorqueAvailable = CAL::toUnsigned(msg.data[2], msg.data[3]) * 0.1f;
                return 1;
            }
            break;
    }
    return 0;
}


//───────────────────────── Parameter I/O ─────────────────────────────────────
// Parameter I/O
void Inverter::requestParamRead(uint16_t address) {
    auto &m = _paramReq;
    m.id     = PARAM_REQ_ID; m.len=8; m.format=0; m.type=0;
    memset(m.data,0,8);
    m.data[0]=CAL::bb0(address); m.data[1]=CAL::bb1(address);
    // byte2=0 => read
}

void Inverter::requestParamWrite(uint16_t address, uint16_t value) {
    auto &m = _paramReq;
    m.id     = PARAM_REQ_ID; m.len=8; m.format=0; m.type=0;
    memset(m.data,0,8);
    m.data[0]=CAL::bb0(address); m.data[1]=CAL::bb1(address);
    m.data[2]=1; // write
    m.data[4]=CAL::bb0(value); m.data[5]=CAL::bb1(value);
}

bool Inverter::handleParamResponse(const CAN_msg_t &msg) {
    if (msg.id != PARAM_RESP_ID) return false;
    _paramResp = msg;
    return true;
}



} // namespace CAL