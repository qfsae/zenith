# EVE_Test_Arduino_PlatformIO Example

This template project uses the [FT800-FT813 library](https://github.com/RudolphRiedel/FT800-FT813/). It is a simplified version of the example in that repository, configured for the Q23 steering display.

The program runs a simple splash screen that shows the Queen's Racing logo briefly on initialization, and then switches to a blank white screen.

## File Structure

Within the `src/` file structure are 3 .c/.cpp files:

| File       | Description                                                  |
| ---------- | ------------------------------------------------------------ |
| main.cpp   | Entry point (main arduino file). setup() and loop() are here |
| tft_data.c | Large data objects ie. bitmaps to be loaded into RAM_G       |
| tft.c      | Display control code                                         |

Most code pertaining to the display belongs in the `tft.c` and `tft_data.c`, with some setup in `main.cpp`.

## `tft_data.c`

Any images required for the program are stored in `tft_data.c`. To use images with the FT813 controller, they must be converted to an EVE compatible format using the [EVE Asset Builder](https://brtchip.com/toolchains/)

1. Save the image as a PNG
2. Use the "Image Utilities" Tab
   - **EVE Chip:** FT813
   - **Output Format:** ARGB1555
   - **Compressed:** on/off optional
   - **Dithering:** on/off optional
3. Use the "bin2c" Tab
   - **Type:** byte
   - **Notation:** hexadecimal
   - **Endianness:** little
4. Copy the array from the generated C file to `tft_data.c`
5. Declare the array in `tft_data.h`

## `tft.c`

Most of the display's control code is contained in `tft.c`. Technically the only required function is `TFT_init()`, but a description of `TFT_display` is included below for completness.

### `TFT_init(void)`

- Signal the display is active
- Set backlight brightness
- Load images into memory using `EVE_cmd_inflate()`

### `TFT_display()`

This function is a blank example that creates a complete display list and transfers it to the command FIFO on the display controller using SPI bursts. The procedure is as follows:

- If the controller is busy, skip
- _Start SPI burst_
- Start a new Display List (`CMD_DLSTART`)
- Clear the screen and previous buffers (`DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG`)
- **Do things with the display!**
- Instruct the coprocessor to show the list (`CMD_DISPLAY`)
- Make the list active (`CMD_SWAP`)
- _End SPI burst_

For an example of loading an image into the display list, see `TFT_splash()`.

## Helpful Links

- [FT813 Programmer's Guide](https://www.ftdichip.com/Support/Documents/ProgramGuides/FT81X_Series_Programmer_Guide.pdf)
- [FT800-FT813 Display Library Repository](https://github.com/RudolphRiedel/FT800-FT813/)
- [NHD-4.3-480272FT-CSXN-CTP Datasheet](https://www.ftdichip.com/Support/Documents/ProgramGuides/FT81X_Series_Programmer_Guide.pdf)
