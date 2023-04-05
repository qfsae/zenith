/**
 * @file main.hpp
 * @author Jacob Chisholm (Jchisholm204.github.io)
 * @brief 
 * @version 0.1
 * @date 2023-02-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "Arduino.h"
#include "SPI.h"
#include "cal.hpp"
#include "can.hpp"

extern CAL::CAL cal;

#define ECU5V 5000
#define ECU0V 0
