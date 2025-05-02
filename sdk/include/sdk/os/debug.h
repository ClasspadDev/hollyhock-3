/**
 * @file
 * @brief Functions useful during debugging.
 * 
 * It can be useful to print text or numbers to the screen whilst debugging,
 * without creating a full GUI, or pause execution and wait for a key press.
 * Thoroughly recommended only for debugging - prefer GUI elements for
 * user-facing input/output!
 *
 * Example:
 * @code{cpp}
 * // Print "Hello, world!" at 0, 0 in black on white
 * Debug_SetCursorPosition(0, 0);
 * Debug_PrintString("Hello, world!", false);
 *
 * // Print "Inverted text" at 1, 1 in white on black
 * Debug_SetCursorPosition(1, 1);
 * Debug_PrintString("Inverted text", true);
 *
 * // Print the number 0x1322 at 3, 7
 * Debug_PrintNumberHex_Word(0x1322, 3, 7);
 *
 * // Print small text with a format string
 * Debug_Printf(20, 20, false, 0, "Format strings are %s in %d!", "cool", 2018);
 * 
 * // Draw the changes we made to VRAM onto the LCD
 * // Defined in sdk/os/lcd.hpp
 * LCD_Refresh();
 *
 * // Block until the user presses a key
 * Debug_WaitKey();
 * @endcode
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * Returns the current position of the cursor in debug text mode.
 * 
 * @param[out] x,y The position of the cursor.
 */
extern void (*Debug_GetCursorPosition)(unsigned int *x, unsigned int *y);

/**
 * Print a formatted string in small debug text mode, either in normal
 * black-on-white or inverted white-on-black. Color inversion occurs if
 * @p invert is true.
 * 
 * Supports most format specifiers.
 * 
 * @param x,y The coordinates to print the formatted string at.
 * @param invert True if the colors used to print the text should be inverted.
 * @param zero Must be passed 0.
 * @param[in] format The format string to use.
 * @param ... The values to substitute into the format string.
 */
extern void (*Debug_Printf)(unsigned int x, unsigned int y, bool invert, int zero, const char *format, ...) __attribute__((format(printf, 5, 6)
#ifndef __clang__
, access(read_only, 5), null_terminated_string_arg(5)
#endif
));

/**
 * Prints the hex representation of a byte (8-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The byte to print.
 * @param x,y The coordinates to print the number at.
 */
extern void (*Debug_PrintNumberHex_Byte)(uint8_t value, unsigned int x, unsigned int y);

/**
 * Prints the hex representation of a dword (32-bit number) at the specified
 * position in debug text mode.
 * 
 * @param value The dword to print.
 * @param x,y The coordinates to print the number at.
 */
extern void (*Debug_PrintNumberHex_Dword)(uint32_t value, unsigned int x, unsigned int y);

/**
 * Prints the hex representation of a nibble (4-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The nibble to print. High 4 bits are ignored.
 * @param x,y The coordinates to print the number at.
 */
extern void (*Debug_PrintNumberHex_Nibble)(uint8_t value, unsigned int x, unsigned int y);

/**
 * Prints the hex representation of a word (16-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The word to print.
 * @param x,y The coordinates to print the number at.
 */
extern void (*Debug_PrintNumberHex_Word)(uint16_t value, unsigned int x, unsigned int y);

/**
 * Prints a string in debug text mode, either in normal black-on-white or
 * inverted white-on-black. Color inversion occurs if \c invert is true.
 * 
 * Returns false if the string did not fit on the screen.
 * 
 * @param[in] string The string to print.
 * @param invert True if the colors used to print the text should be inverted.
 * @return True if writing the string was successful, false otherwise.
 */
extern bool (*Debug_PrintString)(const char *string, bool invert) 
#ifndef __clang__
__attribute__((null_terminated_string_arg(1)))
#endif
;

/**
 * Sets the position of the cursor in debug text mode.
 * 
 * @param x,y The cursor's new position.
 * @return Always returns 0.
 */
extern int (*Debug_SetCursorPosition)(unsigned int x, unsigned int y);

/**
 * Waits until a key is pressed, then returns a number representing the key.
 * Only appears to react to number keys and the Power/Clear key. Returns 0x30
 * to 0x39 for keys 0 to 9, and 0x98 for the Power/Clear key.
 * NEW in 7002: also returns on touch events.
 * 
 * @return A number representing the key that was pressed.
 */
extern int (*Debug_WaitKey)();

#ifdef __cplusplus
}
#endif
