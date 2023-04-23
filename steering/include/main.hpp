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
#include "EasyButton.h"

extern CAL::CAL cal;
extern int logger_gear;

#define ECU5V 5000 //change to 5?
#define ECU0V 0

extern EasyButton rightRed;
extern EasyButton rightBlue;
extern EasyButton leftRed;
extern EasyButton leftBlue;
