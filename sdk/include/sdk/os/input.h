/**
 * @file
 * @brief Functions used to retrieve user input via the touch screen or keypad.
 * 
 * Provides the @ref GetInput method, a function which polls for input events
 * (screen touches and key presses).
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define __UCONCAT(a, b) #a b
#define _UCONCAT(a, b) __UCONCAT(a, b)
#define UCONCAT(b) _UCONCAT(__USER_LABEL_PREFIX__, b)

enum Input_EventType {
	EVENT_KEY = 0x0017,
	EVENT_ACTBAR_RESIZE = 0x1003,
	EVENT_ACTBAR_SWAP = 0x1004,
	EVENT_ACTBAR_ROTATE = 0x1005,
	EVENT_ACTBAR_ESC = 0x3009,
	EVENT_ACTBAR_SETTINGS = 0x300A,
	EVENT_TOUCH = 0x4000,
	EVENT_TIMER = 0x0005
};

enum Input_KeyEventType {
	KEY_PRESSED = 1,
	KEY_HELD = 0x100,
	KEY_RELEASED = 0x40
};

enum Input_Keycode {
	KEYCODE_KEYBOARD = 0x00B3,
	KEYCODE_SHIFT = 0x00A0,
	KEYCODE_BACKSPACE = 0x0097,
	KEYCODE_POWER_CLEAR = 0x0080,
	KEYCODE_UP = 0x0090,
	KEYCODE_DOWN = 0x0091,
	KEYCODE_LEFT = 0x0092,
	KEYCODE_RIGHT = 0x0093,
	KEYCODE_EQUALS = 0x003D,
	KEYCODE_X = 0xEDB8,
	KEYCODE_Y = 0xEDB9,
	KEYCODE_Z = 0xEDBA,
	KEYCODE_POWER = 0x005E,
	KEYCODE_DIVIDE = 0x002F,
	KEYCODE_OPEN_PARENTHESIS = 0x0028,
	KEYCODE_7 = 0x0037,
	KEYCODE_8 = 0x0038,
	KEYCODE_9 = 0x0039,
	KEYCODE_TIMES = 0xEE10,
	KEYCODE_CLOSE_PARENTHESIS = 0x0029,
	KEYCODE_4 = 0x0034,
	KEYCODE_5 = 0x0035,
	KEYCODE_6 = 0x0036,
	KEYCODE_MINUS = 0x002D,
	KEYCODE_COMMA = 0x002C,
	KEYCODE_1 = 0x0031,
	KEYCODE_2 = 0x0032,
	KEYCODE_3 = 0x0033,
	KEYCODE_PLUS = 0x002B,
	KEYCODE_NEGATIVE = 0x001F,
	KEYCODE_0 = 0x0030,
	KEYCODE_DOT = 0x002E,
	KEYCODE_EXP = 0x001D,
	KEYCODE_EXE = 0x0094
};

enum Input_TouchEventType {
	TOUCH_DOWN = 1,
	TOUCH_HOLD_DRAG = 2,
	TOUCH_ACT_BAR = 0x100,
	TOUCH_UP = 0x40
};

enum Input_Scancode {
	ScancodeKeyboard = (7 << 8) | 5,
	ScancodeShift = (7 << 8) | 1,
	ScancodeBackspace = (7 << 8) | 2,
	ScancodeClear = (1 << 8) | 0,
	ScancodeUp = (7 << 8) | 4,
	ScancodeDown = (6 << 8) | 4,
	ScancodeLeft = (6 << 8) | 3,
	ScancodeRight = (7 << 8) | 3,
	ScancodeEquals = (7 << 8) | 6,
	ScancodeX = (6 << 8) | 6,
	ScancodeY = (6 << 8) | 5,
	ScancodeZ = (5 << 8) | 3,
	ScancodePower = (6 << 8) | 2,
	ScancodeDivide = (6 << 8) | 1,
	ScancodeOpenParenthesis = (5 << 8) | 6,
	Scancode7 = (5 << 8) | 5,
	Scancode8 = (5 << 8) | 4,
	Scancode9 = (5 << 8) | 2,
	ScancodeTimes = (5 << 8) | 1,
	ScancodeCloseParenthesis = (4 << 8) | 6,
	Scancode4 = (4 << 8) | 5,
	Scancode5 = (4 << 8) | 4,
	Scancode6 = (4 << 8) | 2,
	ScancodeMinus = (4 << 8) | 1,
	ScancodeComma = (3 << 8) | 6,
	Scancode1 = (3 << 8) | 5,
	Scancode2 = (3 << 8) | 4,
	Scancode3 = (3 << 8) | 2,
	ScancodePlus = (3 << 8) | 1,
	ScancodeNegative = (2 << 8) | 6,
	Scancode0 = (2 << 8) | 5,
	ScancodeDot = (2 << 8) | 4,
	ScancodeEXP = (2 << 8) | 2,
	ScancodeEXE = (2 << 8) | 1
};

/**
 * Information about an input event returned from @ref GetInput. See
 * documentation for individual members for more information.
 */
struct __attribute__((packed)) Input_Event {
	/**
	 * Code representing which event occurred. Events with a value for this
	 * field which do not correspond to a macro beginning with @c EVENT_ must
	 * be ignored.
	 */
	enum Input_EventType type : 16;
	uint16_t zero;

	/**
	 * The data associated with the event. Under no circumstances should the
	 * sub-structure for an event type other than the one communicated by 
	 * @c eventType be accessed.
	 *
	 * Corresponding members:
	 * - @c EVENT_KEY: @c key
	 * - @c EVENT_TOUCH: @c touch_single
	 *
	 * The events @c EVENT_ACTBAR_RESIZE, @c EVENT_ACTBAR_SWAP,
	 * @c EVENT_ACTBAR_ROTATE, @c EVENT_ACTBAR_ESC, and @c EVENT_ACTBAR_SETTINGS
	 * do not report any data.
	 */
	union {
		struct {
			/**
			 * The direction the key traveled. One of @c KEY_PRESSED,
			 * @c KEY_HELD or @c KEY_RELEASED.
			 */
			enum Input_KeyEventType direction : 32;

			/**
			 * The key code for the key. See macros beginning with @c KEYCODE_.
			 */
			enum Input_Keycode keyCode : 16;
		} key;

		struct {
			/**
			 * The direction of the touch. One of @c TOUCH_DOWN,
			 * @c TOUCH_HOLD_DRAG, @c TOUCH_ACT_BAR, or @c TOUCH_UP.
			 */
			enum Input_TouchEventType direction : 32;

			/**
			 * The X position of the cursor, in screen pixels. May be negative
			 * or be greater than or equal to the width of the screen.
			 */
			int32_t p1_x;

			/**
			 * The Y position of the cursor, in screen pixels. May be negative
			 * or be greater than or equal to the height of the screen.
			 */
			int32_t p1_y;

			uint16_t adc_x1;
			uint16_t adc_y1;
			uint16_t adc_z1;
			uint16_t adc_z2;
			uint16_t adc_x2;
			uint16_t adc_y2;

			uint16_t adc_gh;
			uint16_t adc_dm;
		} touch_single;

		struct {
			enum Input_TouchEventType direction : 32;

			int32_t p1_x;
			int32_t p1_y;
			int32_t p1_z;

			int32_t p2_x;
			int32_t p2_y;
			int32_t p2_z;

			uint32_t distance;

			uint16_t adc_x1;
			uint16_t adc_y1;
			uint16_t adc_z1;
			uint16_t adc_z2;
			uint16_t adc_x2;
			uint16_t adc_y2;

			uint16_t adc_gh;
			uint16_t adc_dm;
		} touch_multi;
	} data;
};

/**
 * Polls for any input events.
 *
 * The @p event structure should be zeroed before calling @ref GetInput.
 *
 * @param[out] event The input event's data.
 * @param unknown1 An unknown value. The value @c 0xFFFFFFFF must be supplied.
 * @param unknown2 An unknown value. The value @c 0x10 must be supplied.
 * @return Always returns 0.
 */
extern int (*GetInput)(struct Input_Event *event, uint32_t unknown1, uint32_t unknown2)
#ifndef __clang__
__attribute__((access(write_only, 1)))
#endif
;

extern bool (*Input_GetKeyStateU)(uint16_t *scancode) __asm__ (UCONCAT("Input_GetKeyState"))
#ifndef __clang__
__attribute__((access(read_only, 1)))
#endif
;

/**
 * Returns true if the specified key is currently down.
 * 
 * @param[in] scanCode The scancode of the key to check.
 * @returns True if the key is down, false otherwise.
 */
static inline __attribute__((always_inline)) bool Input_GetKeyState(enum Input_Scancode scanCode) {
	return Input_GetKeyStateU((uint16_t *)&scanCode);
}

/**
 * Returns true if any key on the keyboard is currently down.
 * 
 * @returns True if a key is pressed, false otherwise.
 */
extern bool (*Input_IsAnyKeyDown)();

#undef __UCONCAT
#undef _UCONCAT
#undef UCONCAT

#ifdef __cplusplus
}
#endif
