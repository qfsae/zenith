// src/main.cpp

#include <Arduino.h>
#include <SPI.h>
#include <mcp2515.h>

#include "cal.hpp"
#include "commands.hpp"

static const int CS_PIN = 9;
static MCP2515 mcp2515(CS_PIN);
static CAL::CAL    cal;

// ─── send helper ──────────────────────────────────────────────────────────────
static void sendCan(const CAL::CAN_msg_t &msg) {
  struct can_frame f{};
  f.can_id  = msg.id;
  f.can_dlc = msg.len;
  memcpy(f.data, msg.data, msg.len);
  if (mcp2515.sendMessage(&f) != MCP2515::ERROR_OK) {
    Serial.println(F("[ERR] CAN send failed"));
  }
}

// ─── Menus ───────────────────────────────────────────────────────────────────
enum class State { DEVICE, ACTION, QUERY, SET };
static State state = State::DEVICE;

static void showDevice() {
  Serial.println(F("\n=== Device ==="));
  Serial.println(F("1) Inverter"));
  Serial.print  (F("> "));
}
static void showAction() {
  Serial.println(F("\n=== Action ==="));
  Serial.println(F("1) Query telemetry"));
  Serial.println(F("2) Command / Param I/O"));
  Serial.println(F("b) Back"));
  Serial.print  (F("> "));
}
static void showQuery() {
  Serial.println(F("\n=== Query ==="));
  Serial.println(F("1) DC Bus Voltage"));
  Serial.println(F("2) Phase A Current"));
  Serial.println(F("3) Motor Speed"));
  Serial.println(F("4) Module A Temp"));
  Serial.println(F("5) Inverter Hot‑Spot Temp"));
  Serial.println(F("b) Back"));
  Serial.print  (F("> "));
}
static void showSet() {
  Serial.println(F("\n=== Set / Param ==="));
  Serial.println(F("1) Send torque/speed command"));
  Serial.println(F("2) Read EEPROM param"));
  Serial.println(F("3) Write EEPROM param"));
  Serial.println(F("b) Back"));
  Serial.print  (F("> "));
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  Serial.println(F("=== Dashboard ==="));
  showDevice();
}

void loop() {
  // 1) Drain incoming CAN → CAL parser
  struct can_frame rx;
  if (mcp2515.readMessage(&rx) == MCP2515::ERROR_OK) {
    CAL::CAN_msg_t m{};
    m.id     = rx.can_id;
    m.len    = rx.can_dlc;
    m.ch     = 0;
    m.format = 0;
    m.type   = 0;
    memcpy(m.data, rx.data, rx.can_dlc);
    cal.updatePackage(m);
  }

  // 2) Serial‑driven menu
  if (!Serial.available()) return;
  String s = Serial.readStringUntil('\n');
  s.trim();
  if (s == "") return;

  // Back?
  if (s.equalsIgnoreCase("b")) {
    if (state == State::ACTION) {
      state = State::DEVICE;
      showDevice();
    }
    else if (state == State::QUERY || state == State::SET) {
      state = State::ACTION;
      showAction();
    }
    return;
  }

  int c = s.toInt();
  switch (state) {
    case State::DEVICE:
      if (c == 1) {
        state = State::ACTION;
        showAction();
      } else {
        Serial.println(F("Invalid"));
        showDevice();
      }
      break;

    case State::ACTION:
      if (c == 1) {
        state = State::QUERY;
        showQuery();
      }
      else if (c == 2) {
        state = State::SET;
        showSet();
      }
      else {
        Serial.println(F("Invalid"));
        showAction();
      }
      break;

    case State::QUERY:
      if (!cal.inverter.isConnected()) {
        Serial.println(F("⚠  No data from inverter yet"));
      }
      else if (cal.inverter.isStale(500)) {  // e.g. 500 ms timeout
        Serial.println(F("⚠  Inverter data is stale"));
      }
      else {
        switch (c) {
          case 1:
            Serial.print(F("DC Bus Voltage = "));
            Serial.print(cal.inverter.getDCBusVoltage());
            Serial.println(F(" V"));
            break;
          case 2:
            Serial.print(F("Phase A Current = "));
            Serial.print(cal.inverter.getPhaseACurrent());
            Serial.println(F(" A"));
            break;
          case 3:
            Serial.print(F("Motor Speed = "));
            Serial.print(cal.inverter.getMotorSpeed());
            Serial.println(F(" RPM"));
            break;
          case 4:
            Serial.print(F("Module A Temp = "));
            Serial.print(cal.inverter.getModuleATemp());
            Serial.println(F(" °C"));
            break;
          case 5:
            Serial.print(F("Inv Hot‑Spot = "));
            Serial.print(cal.inverter.getInverterHotSpotTemp());
            Serial.println(F(" °C"));
            break;
          default:
            Serial.println(F("Invalid"));
        }
      }
      showQuery();
      break;

    case State::SET:
      if (c == 1) {
        // collect
        Serial.print(F("Torque (x10): "));
        int tq  = Serial.parseInt();
        Serial.print(F("Speed (RPM): "));
        int sp  = Serial.parseInt();
        Serial.print(F("Dir (0/1): "));
        bool dir = (Serial.parseInt() != 0);
        Serial.print(F("Run (0/1): "));
        bool run = (Serial.parseInt() != 0);

        // pack & send
        cal.updateVar(CAL::CMD::TORQUE,    tq);
        cal.updateVar(CAL::CMD::SPEED,     sp);
        cal.updateVar(CAL::CMD::DIRECTION, dir);
        cal.updateVar(CAL::CMD::RUN,       run);
        sendCan(cal.package(CAL::CMD::TORQUE));
        Serial.println(F("Command sent."));
      }
      else if (c == 2 || c == 3) {
        bool isWrite = (c == 3);
        Serial.print(F("Param addr (hex): 0x"));
        long addr = strtol(Serial.readStringUntil('\n').c_str(), NULL, 16);

        if (!isWrite) {
          cal.inverter.requestParamRead(addr);
        } else {
          Serial.print(F("Value   (hex): 0x"));
          long val = strtol(Serial.readStringUntil('\n').c_str(), NULL, 16);
          cal.inverter.requestParamWrite(addr, val);
        }

        // send param‑request
        sendCan(cal.inverter.paramPackage());
        Serial.println(F("Waiting for reply…"));

        // wait up to 500ms
        unsigned long t0 = millis();
        while (millis() - t0 < 500) {
          if (mcp2515.readMessage(&rx) == MCP2515::ERROR_OK) {
            CAL::CAN_msg_t rm{ rx.can_id, {0}, uint8_t(rx.can_dlc), 0, 0, 0 };
            memcpy(rm.data, rx.data, rx.can_dlc);
            if (cal.inverter.handleParamResponse(rm)) {
              auto &r = cal.inverter.lastParamResponse();
              uint16_t echo   = r.data[0] | (r.data[1] << 8);
              uint8_t  status = r.data[2];
              uint16_t data   = r.data[4] | (r.data[5] << 8);
              Serial.print(F("Addr=0x"));    Serial.println(echo, HEX);
              Serial.print(F("Status="));    Serial.println(status);
              Serial.print(F("Value=0x"));   Serial.println(data, HEX);
              break;
            }
          }
        }
      }
      else {
        Serial.println(F("Invalid"));
      }
      showSet();
      break;
  }
}