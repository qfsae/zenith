#ifndef FT813_H
#define FT813_H

#define RAM_DL      0x300000    // start of display list in RAM
#define RAM_REG     0x302000    // start of registers in RAM
//#define REG_HCYCLE      (RAM_REG + 0x2C)
#define REG_HCYCLE      0x30202C
#define REG_HOFFSET     0x302030
#define REG_HSYNC0      0x302038
#define REG_HSYNC1      0x30203C
#define REG_VCYCLE      0x302040
#define REG_VOFFSET     0x302044
#define REG_VSYNC0      0x30204C
#define REG_VSYNC1      0x302050
#define REG_SWIZZLE     0x302064
#define REG_PCLK_POL    0x30206C
#define REG_PCLK        0x302070
#define REG_CSPREAD     0x302068
#define REG_HSIZE       0x302034
#define REG_VSIZE       0x302048
#define REG_DLSWAP      0x302054
#define REG_GPIO_DIR    0x302090
#define REG_GPIO        0x302094

#endif // FT813_H
