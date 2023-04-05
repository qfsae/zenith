
#ifndef TFT_H
#define TFT_H

// Debug Stats
extern uint16_t last_button_state_3, last_button_state_4, steering_3, steering_4, gear_position;

#ifdef __cplusplus
extern "C" {
#endif

void TFT_init(void);
void TFT_touch(void);
void TFT_display(void);
void TFT_splash(void);

#ifdef __cplusplus
}
#endif

#endif /* TFT_H */
