/**
 * @file tft.h
 * @author Jacob Chisholm (jchisholm204.github.io)
 * @author Logan Lim
 * @author Ethan Peterson (portfolio.petetech.net)
 * @brief TFT Display Code
 * @version 1.2
 * @date 2023-01-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef TFT_H
#define TFT_H

#ifdef __cplusplus
extern "C" {
#endif

// Quick hack to extern the CAL data over to tft.c
typedef struct {
    int rpm;
    int speed;
    int engine_temp;
    int coolant_temp;
    int tps;
    bool status;
    int gear_pos;
} DataHolder;

void TFT_init(void);
void TFT_touch(void);
void TFT_display(void);
void TFT_splash(void);

extern DataHolder ecu_data;

#ifdef __cplusplus
}
#endif

#endif /* TFT_H */
