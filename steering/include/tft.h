
#ifndef TFT_H
#define TFT_H

// Debug Stats
extern uint16_t num_profile_a, num_profile_b;

#ifdef __cplusplus
extern "C" {
#endif

// Some pre-defined colours
#define RED     0xff0000UL
#define ORANGE  0xffa500UL
#define GREEN   0x00ff00UL
#define BLUE    0x0000ffUL
#define BLUE_1  0x5dade2L
#define YELLOW  0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE  0x800080UL
#define WHITE   0xffffffUL
#define BLACK   0x000000UL

typedef void (*tft_callback)();

void TFT_init(void);
void TFT_touch(void);
void TFT_display(tft_callback f);
void TFT_splash(void);

#ifdef __cplusplus
}
#endif

#endif /* TFT_H */
