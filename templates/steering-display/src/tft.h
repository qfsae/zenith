
#ifndef TFT_H
#define TFT_H

// Debug Stats
extern uint16_t num_profile_a, num_profile_b;

#ifdef __cplusplus
extern "C" {
#endif

void TFT_init(void);
void TFT_touch(void);
void TFT_display(void);
void TFT_splash(void);

// TPS test
extern uint8_t tps;

#ifdef __cplusplus
}
#endif

#endif /* TFT_H */
