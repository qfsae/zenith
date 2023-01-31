/**
 * @file tft_data.h
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @author Logan Lim
 * @author Ethan Peterson (portfolio.petetech.net)
 * @brief TFT Image Data Externs
 * @version 1.2
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef TFT_DATA_H
#define TFT_DATA_H
#include <stdint.h>

#if !defined(PROGMEM)
	#define PROGMEM
#endif

extern const uint8_t logo[5298] PROGMEM, center_n[1075] PROGMEM, center_1[383] PROGMEM, center_2[673] PROGMEM, center_3[893]PROGMEM, center_4[614]PROGMEM, center_5[879]PROGMEM;
#endif /* TFT_DATA_H */
