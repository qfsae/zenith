// lib/CAL25/inverter.hpp
#ifndef INVERTER_HPP
#define INVERTER_HPP

#include <Arduino.h>
#include <string.h>
#include "data_structs.hpp"

namespace CAL {

/**
 * @brief Parser for CM-series inverter CAN broadcasts (0x0A0–0x0B1).
 */
class Inverter {
public:
    Inverter() = default;

    //  Decode one CAN frame
    int handleMessage(const CAN_msg_t &msg);

    /// Returns true as soon as the very first broadcast arrives.
    bool isConnected() const { return hasSeenAny; }

    /// Returns true if no message has arrived for more than `timeoutMs`.
    bool isStale(uint32_t timeoutMs) const {
        return hasSeenAny && (millis() - lastRxMs > timeoutMs);
    }


    //───────────────────────── Parameter I/O ─────────────────────────────────────
    /**
     * Build a read (mode=0) request at ID = INVERTER + 0xC1.
     */
    void requestParamRead(uint16_t address);
    /**
     * Build a write (mode=1) request at ID = INVERTER + 0xC1.
     */
    void requestParamWrite(uint16_t address, uint16_t value);
    CAN_msg_t &paramPackage() { return _paramReq; }

    /**
     * Catch & store the parameter response at ID = INVERTER + 0xC2.
     * Returns true if msg was that response.
     */
    bool handleParamResponse(const CAN_msg_t &msg);
    CAN_msg_t &lastParamResponse() { return _paramResp; }


    
    // ——— Getters —————————————————————————————————————————————————————

    float getModuleATemp()           const { return moduleATemp; }
    float getModuleBTemp()           const { return moduleBTemp; }
    float getModuleCTemp()           const { return moduleCTemp; }
    float getGDBTemp()               const { return gdbTemp; }

    float getControlBoardTemp()      const { return controlBoardTemp; }
    float getRTD1Temp()              const { return rtd1Temp; }
    float getRTD2Temp()              const { return rtd2Temp; }
    float getMotorHotSpotTemp()      const { return motorHotSpotTemp; }

    float getCoolantTemp()           const { return coolantTemp; }
    float getInverterHotSpotTemp()   const { return inverterHotSpotTemp; }
    float getMotorTemp()             const { return motorTemp; }
    float getTorqueShudder()         const { return torqueShudder; }

    float getAnalogInput(uint8_t i)  const {
        return (i < 6) ? analogInputs[i] : 0.0f;
    }
    bool  getDigitalInput(uint8_t i) const {
        return (i < 8) ? digitalInputs[i] : false;
    }

    float getMotorAngleElectrical()  const { return motorAngleElectrical; }
    float getMotorSpeed()            const { return motorSpeed; }
    float getElectricalOutputFreq()  const { return electricalOutputFrequency; }
    float getDeltaResolverFiltered() const { return deltaResolverFiltered; }

    float getPhaseACurrent()         const { return phaseACurrent; }
    float getPhaseBCurrent()         const { return phaseBCurrent; }
    float getPhaseCCurrent()         const { return phaseCCurrent; }
    float getDCBusCurrent()          const { return dcBusCurrent; }

    float getDCBusVoltage()          const { return dcBusVoltage; }
    float getOutputVoltage()         const { return outputVoltage; }
    float getVabVdVoltage()          const { return vabVdVoltage; }
    float getVbcVqVoltage()          const { return vbcVqVoltage; }

    float getVd_ff()                 const { return vd_ff; }
    float getVq_ff()                 const { return vq_ff; }
    float getIdFeedback()            const { return idFeedback; }
    float getIqFeedback()            const { return iqFeedback; }

    float getRefVoltage1_5()         const { return refVoltage1_5; }
    float getRefVoltage2_5()         const { return refVoltage2_5; }
    float getRefVoltage5_0()         const { return refVoltage5_0; }
    float getRefVoltage12_0()        const { return refVoltage12_0; }

    uint8_t getVsmState()            const { return vsmState; }
    uint16_t getPWMFrequency()       const { return pwmFrequency; }
    uint8_t getInverterState()       const { return inverterState; }
    uint8_t getRelayStates()         const { return relayStates; }
    bool    inTorqueMode()           const { return inverterRunMode == false; }
    bool    isSelfSensingAssistOn()  const { return selfSensingAssistEnable; }
    bool    getDirectionCommand()    const { return directionCommand; }
    bool    isBMSActive()            const { return bmsActive; }
    bool    isBMSLimitingMotorTorque() const { return bmsLimitingMotorTorque; }
    bool    isMaxSpeedLimited()      const { return limitMaxSpeed; }
    bool    isHotSpotInverterLimited() const { return limitHotSpotInverter; }
    bool    isLowSpeedLimited()      const { return lowSpeedLimiting; }
    bool    isCoolantDerated()       const { return limitCoolantDerating; }
    bool    isStallBurstLimited()    const { return limitStallBurstModel; }

    uint32_t getPOSTFaultLo()        const { return postFaultLo; }
    uint32_t getPOSTFaultHi()        const { return postFaultHi; }
    uint32_t getRunFaultLo()         const { return runFaultLo; }
    uint32_t getRunFaultHi()         const { return runFaultHi; }

    float   getCommandedTorque()     const { return commandedTorque; }
    float   getTorqueFeedback()      const { return torqueFeedback; }
    uint32_t getPowerOnTimer()       const { return powerOnTimer; }

    float   getModulationIndex()     const { return modulationIndex; }
    float   getFluxWeakeningOutput() const { return fluxWeakeningOutput; }
    float   getIdCommandCurrent()    const { return idCommandCurrent; }
    float   getIqCommandCurrent()    const { return iqCommandCurrent; }

    uint16_t getEEPROMVersion()      const { return eepromVersion; }
    uint16_t getSoftwareVersion()    const { return softwareVersion; }
    uint16_t getDateCodeMMDD()       const { return dateCodeMMDD; }
    uint16_t getDateCodeYYYY()       const { return dateCodeYYYY; }

    float   getHS_TorqueCommand()    const { return highSpeedTorqueCommand; }
    float   getHS_TorqueFeedback()   const { return highSpeedTorqueFeedback; }
    float   getHS_MotorSpeed()       const { return highSpeedMotorSpeed; }
    float   getHS_DCBusVoltage()     const { return highSpeedDCBusVoltage; }

    float   getMotorTorqueAvailable() const { return motorTorqueAvailable; }
    float   getRegenTorqueAvailable() const { return regenTorqueAvailable; }

private:
    bool     hasSeenAny{false};
    uint32_t lastRxMs{0};

    // IDs
    static constexpr uint32_t CMD_ID        = CAN_ID::INVERTER + 0xC0u;
    static constexpr uint32_t PARAM_REQ_ID  = CAN_ID::INVERTER + 0xC1u;
    static constexpr uint32_t PARAM_RESP_ID = CAN_ID::INVERTER + 0xC2u;


    // Storage for command & param frames
    CAN_msg_t _cmd      {};
    CAN_msg_t _paramReq {};
    CAN_msg_t _paramResp{};


    // Backing fields
    float moduleATemp{0}, moduleBTemp{0}, moduleCTemp{0}, gdbTemp{0};
    float controlBoardTemp{0}, rtd1Temp{0}, rtd2Temp{0}, motorHotSpotTemp{0};
    float coolantTemp{0}, inverterHotSpotTemp{0}, motorTemp{0}, torqueShudder{0};
    float analogInputs[6]{};
    bool  digitalInputs[8]{};
    float motorAngleElectrical{0}, motorSpeed{0}, electricalOutputFrequency{0}, deltaResolverFiltered{0};
    float phaseACurrent{0}, phaseBCurrent{0}, phaseCCurrent{0}, dcBusCurrent{0};
    float dcBusVoltage{0}, outputVoltage{0}, vabVdVoltage{0}, vbcVqVoltage{0};
    float vd_ff{0}, vq_ff{0}, idFeedback{0}, iqFeedback{0};
    float refVoltage1_5{0}, refVoltage2_5{0}, refVoltage5_0{0}, refVoltage12_0{0};
    uint8_t vsmState{0}, inverterState{0}, relayStates{0};
    uint16_t pwmFrequency{0};
    bool inverterRunMode{false}, selfSensingAssistEnable{false};
    bool directionCommand{false}, bmsActive{false}, bmsLimitingMotorTorque{false};
    bool limitMaxSpeed{false}, limitHotSpotInverter{false}, lowSpeedLimiting{false};
    bool limitCoolantDerating{false}, limitStallBurstModel{false};
    uint32_t postFaultLo{0}, postFaultHi{0}, runFaultLo{0}, runFaultHi{0};
    float commandedTorque{0}, torqueFeedback{0};
    uint32_t powerOnTimer{0};
    float modulationIndex{0}, fluxWeakeningOutput{0};
    float idCommandCurrent{0}, iqCommandCurrent{0};
    uint16_t eepromVersion{0}, softwareVersion{0}, dateCodeMMDD{0}, dateCodeYYYY{0};
    float highSpeedTorqueCommand{0}, highSpeedTorqueFeedback{0};
    float highSpeedMotorSpeed{0}, highSpeedDCBusVoltage{0};
    float motorTorqueAvailable{0}, regenTorqueAvailable{0};
};

} // namespace CAL

#endif // INVERTER_HPP