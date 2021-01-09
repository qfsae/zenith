/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */
#include "gfx.h"

#if GFX_USE_GDISP

#if defined(GSCREEN_HEIGHT)
	#warning "G This low level driver does not support setting a screen size. It is being ignored."
	#undef GISP_SCREEN_HEIGHT
#endif
#if defined(GSCREEN_WIDTH)
	#warning "G This low level driver does not support setting a screen size. It is being ignored."
	#undef GSCREEN_WIDTH
#endif

#define GDISP_DRIVER_VMT			GMT_FT813
#include "gdisp_lld_config.h"
#include "src/gdisp/gdisp_driver.h"

#include "board_FT813.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#ifndef GSCREEN_HEIGHT
	#define GSCREEN_HEIGHT		272
#endif
#ifndef GSCREEN_WIDTH
	#define GSCREEN_WIDTH		480
#endif
#ifndef GINITIAL_CONTRAST
	#define GINITIAL_CONTRAST	100
#endif
#ifndef GINITIAL_BACKLIGHT
	#define GINITIAL_BACKLIGHT	100
#endif

#include "FT813.h"

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

uint8_t rd8(uint32_t addr) {
  acquire_bus(g);
  uint8_t read = 0x00;

  uint8_t tx[] = {
    (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    0x00                            // dummy byte
  };

  write_data(g, tx[0]);
  write_data(g, tx[1]);
  write_data(g, tx[2]);
  write_data(g, tx[2]);
  uint8_t byte1 = write_data(g, 0x00);

  read = byte1;
  release_bus(g);
  return read;
}
uint16_t rd16(uint32_t addr) {
  acquire_bus(g);
  uint8_t read = 0x0000;

  uint8_t tx[] = {
    (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    0x00                            // dummy byte
  };

  write_data(g, tx[0]);
  write_data(g, tx[1]);
  write_data(g, tx[2]);
  write_data(g, tx[2]);
  uint8_t byte1 = write_data(g, 0x00);
  uint8_t byte2 = write_data(g, 0x00);

  read = (byte2<<8) | byte1;
  release_bus(g);
  return read;
}
uint32_t rd32(uint32_t addr) {
  acquire_bus(g);
  uint8_t read = 0x00000000;

  uint8_t tx[] = {
    (0x00 | ((addr >> 16) & 0xFF)), // read code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    0x00                            // dummy byte
  };

  write_data(g, tx[0]);
  write_data(g, tx[1]);
  write_data(g, tx[2]);
  write_data(g, tx[2]);
  uint8_t byte1 = write_data(g, 0x00);
  uint8_t byte2 = write_data(g, 0x00);
  uint8_t byte3 = write_data(g, 0x00);
  uint8_t byte4 = write_data(g, 0x00);

  read = (byte4<<24) | (byte3<<16) | (byte2<<8) | byte1;
  release_bus(g);
  return read;
}

void wr8(uint32_t addr, uint8_t data) {
  acquire_bus(g);
  uint8_t tx[4] = {
    (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    (data >> 0) & 0xFF              // first byte of data
  };

  for(int j = 0; j < 4; j++) {
    write_data(g, tx[j]);
  } 
  release_bus(g);
}
void wr16(uint32_t addr, uint16_t data) {
  acquire_bus(g);
  uint8_t tx[5] = {
    (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    (data >> 0) & 0xFF,             // first byte of data
    (data >> 8) & 0xFF              // second byte of data
  };

  for(int j = 0; j < 5; j++) {
    write_data(g, tx[j]);
  } 
  release_bus(g);
}
void wr32(uint32_t addr, uint32_t data) {
  acquire_bus(g);
  uint8_t tx[7] = {
    (0x80 | ((addr >> 16) & 0xFF)), // write code, upper 6 bits of addr
    (addr >> 8) & 0xFF,             // bits 8-15 of addr
    (addr >> 0) & 0xFF,             // bits 0-7 of addr
    (data >> 0) & 0xFF,            // first byte of data
    (data >> 8) & 0xFF,            // second byte of data
    (data >> 16) & 0xFF,             // third byte of data
    (data >> 24) & 0xFF              // fourth byte of data
  };

  for(int j = 0; j < 7; j++) {
    write_data(g, tx[j]);
  } 
  release_bus(g);
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

LLDSPEC bool_t gdisp_lld_init(GDisplay *g) {
	// No private area for this controller
	g->priv = 0;

	// Initialise the board interface
	init_board(g);

	// Hardware reset
	/*  TODO IMPLEMENT HARDWARE RESET */

	// Get the bus for the special active command
	acquire_bus(g);
  write_data(g, 0x00);    // 3x empty write sends ACTIVE signal
  write_data(g, 0x00);    // this is a special host command, which is not yet implemented
  write_data(g, 0x00);    // doing it manually here for now
	release_bus(g);
  
  debug_msg(0);

  /* Configure Display registers */
  wr16(REG_HCYCLE, 548);
  wr16(REG_HOFFSET, 43);
  wr16(REG_HSYNC0, 0); 
  wr16(REG_HSYNC1, 41); 
  wr16(REG_VCYCLE, 292); 
  wr16(REG_VOFFSET, 12); 
  wr16(REG_VSYNC0, 0); 
  wr16(REG_VSYNC1, 10); 
  wr8(REG_SWIZZLE, 0); 
  wr16(REG_PCLK_POL, 1); 
  wr8(REG_CSPREAD, 1); 
  wr16(REG_HSIZE, 480); 
  wr16(REG_VSIZE, 272);

  debug_msg(1);

  /* Write first display list */
  wr32(RAM_DL+0, 0x02000000); // CLEAR_COLOR_RGB(0,0,0) command
  wr32(RAM_DL+4, 0x26000007); // CLEAR(1,1,1) command

  // just for shits
  wr32(RAM_DL+8, 0x1F000001); // BEGIN(BITMAPS);
  wr32(RAM_DL+12, 0x9B86EFC6);  // VERTEX2II(220,110,31,'F'));
  wr32(RAM_DL+16, 0x21000000);  // END()           
  // end just for shits

  wr32(RAM_DL+20, 0x00000000); // DISPAY();

  debug_msg(2);

  /* Display List Swap */
  gdisp_lld_flush(g);     // render new screen when ready
  
  debug_msg(3);

  /* Enable backlight control */
  uint8_t gpio_dir = rd8(REG_GPIO_DIR);
  wr8(REG_GPIO_DIR, (0x80 | gpio_dir));
  uint8_t gpio = rd8(REG_GPIO);
  wr8(REG_GPIO, (0x080 | gpio));

  debug_msg(4);

  /* Divide main clock for PCLK; start video */
  wr8(REG_PCLK, 5);

  debug_msg(5);

  // Finish Init
  post_init_board(g);

	/* Turn on the back-light */
	set_backlight(g, GINITIAL_BACKLIGHT);

	/* Initialise the Gstructure */
	g->g.Width = GSCREEN_WIDTH;
	g->g.Height = GSCREEN_HEIGHT;
	g->g.Orientation = GDISP_ROTATE_0;
	g->g.Powermode = powerOn;
	g->g.Backlight = GINITIAL_BACKLIGHT;
	g->g.Contrast = GINITIAL_CONTRAST;
	return TRUE;
}

#if GDISP_HARDWARE_FLUSH
  LLDSPEC void gdisp_lld_flush(GDisplay *g) {
    debug_msg(6);
    wr8(REG_DLSWAP, 0x01);
  }
#endif

#if GDISP_HARDWARE_DRAWPIXEL
  LLDSPEC void gdisp_lld_draw_pixel(GDisplay *g) {
    debug_msg(7);
  }
#endif

#endif /* GFX_USE_GDISP*/
