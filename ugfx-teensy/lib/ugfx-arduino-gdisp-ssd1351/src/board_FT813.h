/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#define init_board(g)          FT813_init_board()
#define post_init_board(g)     FT813_post_init_board()
#define setpin_reset(g, state) FT813_setpin_reset(state)
#define acquire_bus(g)         FT813_acquirebus()
#define release_bus(g)         FT813_releasebus()
#define write_cmd(g, cmd)      
#define write_data(g, data)    FT813_write_data(data)
#define set_backlight(g, perent)
#define setreadmode(g)
#define setwritemode(g)
#define read_data(g) 0
#define debug_msg(num)          debug_println(num)

#ifdef __cplusplus
extern "C" {
#endif

void FT813_init_board(void);
void FT813_post_init_board(void);
void FT813_setpin_reset(int state);
void FT813_acquirebus(void);
void FT813_releasebus(void);
uint8_t FT813_write_data(uint8_t data);

void debug_println(int num);

#ifdef __cplusplus
}
#endif



#endif /* _GDISP_LLD_BOARD_H */
