/**
 * @file
 * @brief Functions for interacting with the LCD and VRAM.
 *
 * Different levels of indirection are availiable to write to VRAM (direct
 * memory access, getter/setters and palette-based drawing). The contents of
 * VRAM are not automatically drawn to the LCD, and must be rendered with
 * @ref LCD_Refresh. 
 * 
 * Example: drawing a 30x50 rectangle at 10, 20 in purple
 * @code{cpp}
 * uint16_t *vram = LCD_GetVRAMAddress();
 * int width, height;
 * LCD_GetSize(&width, &height);
 *
 * for (int y = 0; y < 50; ++y) {
 *     for (int x = 0; x < 30; ++x) {
 *         vram[(x + 10) + (y + 20) * width] = RGB_TO_RGB565(0x1F, 0x3B, 0x08);
 *     }
 * }
 *
 * // Put our changes on the display
 * LCD_Refresh();
 * @endcode
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define __UCONCAT(a, b) #a b
#define _UCONCAT(a, b) __UCONCAT(a, b)
#define UCONCAT(b) _UCONCAT(__USER_LABEL_PREFIX__, b)

/**
 * Passed to @ref LCD_SetPixelFromPalette.
 */
enum LCD_Palette {
	PALETTE_BLACK = 0,
	PALETTE_BLUE = 1,
	PALETTE_GREEN = 2,
	PALETTE_CYAN = 3,
	PALETTE_RED = 4,
	PALETTE_MAGENTA = 5,
	PALETTE_YELLOW = 6,
	PALETTE_WHITE = 7
};

/**
 * Passed to @ref LCD_SendCommand.
 */
enum LCD_Command {
	COMMAND_SET_X_WINDOW = 0x2A,
	COMMAND_SET_Y_WINDOW = 0x2B,
	COMMAND_PREPARE_FOR_DRAW_DATA = 0x2C,
	COMMAND_READ_DRAW_DATA = 0x2E
};

volatile uint16_t *const lcd_data_port = (volatile uint16_t *)0xB4000000;

/**
 * Converts three RGB values into one RGB565 value.
 *
 * @param r The red component, between 0 and 31 (0x1F) inclusive.
 * @param g The green component, between 0 and 63 (0x3F) inclusive.
 * @param b The blue component, between 0 and 31 (0x1F) inclusive.
 * @return The specified color, in RGB565 format.
 */
#define RGB_TO_RGB565(r, g, b) ( \
	((r & 0x1F) << 11) | \
	((g & 0x3F) << 5) | \
	(b & 0x1F) \
)

/**
 * Extracts the red component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The red component.
 */
#define RGB565_TO_R(rgb565) ((rgb565 >> 11) & 0x1F)

/**
 * Extracts the green component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The green component.
 */
#define RGB565_TO_G(rgb565) ((rgb565 >> 5) & 0x3F)

/**
 * Extracts the blue component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The blue component.
 */
#define RGB565_TO_B(rgb565) (rgb565 & 0x1F)

/**
 * Clears the LCD. Fills VRAM with white, but does not refresh the LCD.
 */
extern void (*LCD_ClearScreen)();

/**
 * Returns the color of a pixel. This is not necessarily the color which is
 * being displayed on the LCD, but instead is the color specified for the pixel
 * in the VRAM buffer. Color is in RGB565 format.
 * 
 * @param x,y The coordinates of the pixel.
 * @return The color of the pixel, in RGB565 format.
 */
extern uint16_t (*LCD_GetPixel)(unsigned int x, unsigned int y);

/**
 * Retrieves the size, in pixels, of the LCD.
 * 
 * @param[out] width,height The LCD's size.
 */
extern void (*LCD_GetSize)(unsigned int *width, unsigned int *height);

extern uint16_t *(*_FP_LCD_GetVRAMAddress)() __asm__ (UCONCAT("LCD_GetVRAMAddress"));

/**
 * Returns a pointer to the video RAM. Video RAM is composed of
 * <tt>width * height</tt> 16-bit integers (in row-major order) representing the
 * color at each pixel, in RGB565 format.
 *
 * @return A pointer to the VRAM.
 */
static inline __attribute__((const, always_inline)) uint16_t *LCD_GetVRAMAddress() {
	return _FP_LCD_GetVRAMAddress();
}

/**
 * Pushes the content of the VRAM to the LCD.
 */
extern void (*LCD_Refresh)();

/**
 * Sets the color of a pixel. The result of this operation will not be visible
 * until @ref LCD_Refresh is called. Color is in RGB565 format.
 * 
 * @param x,y The coordinate of the pixel.
 * @param color The color to set the pixel, in RGB565 format.
 */
extern void (*LCD_SetPixel)(unsigned int x, unsigned int y, uint16_t color);

extern void (*LCD_SetPixelFromPaletteU)(unsigned int x, unsigned int y, uint8_t index) __asm__ (UCONCAT("LCD_SetPixelFromPalette"));

/**
 * Sets the color of a pixel, from a pre-defined palette. Result is not visible
 * until @ref LCD_Refresh is called. See @ref LCD_Palette.
 *
 * @param x,y The coordinate of the pixel.
 * @param color The color in the palette to use.
 */
static inline __attribute__((always_inline)) void LCD_SetPixelFromPalette(unsigned int x, unsigned int y, enum LCD_Palette color) {
	LCD_SetPixelFromPaletteU(x, y, (uint8_t)color);
}

/**
 * Backs up the current contents of VRAM.
 * 
 * Should be used to prevent display corruption when writing directly to VRAM.
 * 
 * Used in conjunction with @ref LCD_VRAMRestore.
 */
extern void (*LCD_VRAMBackup)();

/**
 * Restores the backed up contents of VRAM. The restored content is not
 * displayed until @ref LCD_Refresh is called.
 * 
 * Used in conjunction with @ref LCD_VRAMBackup.
 */
extern void (*LCD_VRAMRestore)();

extern void (*LCD_SendCommandU)(uint16_t command) __asm__ (UCONCAT("LCD_SendCommand"));

/**
 * Switches to command mode, send the command and switches back to data mode
 */
static inline __attribute__((always_inline)) void LCD_SendCommand(enum LCD_Command command) {
	LCD_SendCommandU(command);
}

/**
 * Sets the region of the next display update. Needs to be called every time.
 * Usefull for partial updates and/or DMA.
 *
 * You probably want to use this in conjunction with @ref LCD_SendCommand
 */
extern void (*LCD_SetDrawingBounds)(unsigned int xstart, unsigned int xend, unsigned int ystart, unsigned int yend);

#undef __UCONCAT
#undef _UCONCAT
#undef UCONCAT

#ifdef __cplusplus
}
#endif
