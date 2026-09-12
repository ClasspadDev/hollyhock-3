/**
 * @file
 * @brief Functions used for sending and receiving data via USB.
 */

// This works exactly the same as the calls on the cg50.
// These implementations are similar to:
// https://github.com/Jonimoose/libfxcg/blob/master/include/fxcg/usb.h

#pragma once

#ifdef __cplusplus
extern "C" {
#elif __STDC_VERSION__ < 202311L
#define constexpr
#endif

#ifndef __clang__
#define ro(...) __attribute__((access(read_only, __VA_ARGS__)))
#define rw(...) __attribute__((access(read_write, __VA_ARGS__)))
#define wo(...) __attribute__((access(write_only, __VA_ARGS__)))
#else
#define ro(...)
#define rw(...)
#define wo(...)
#endif

#include <stdint.h>
#include <stdbool.h>

static constexpr const short MODE_USB = 0x20;

/**
 * Opens the USB port.
 * Abortable by pressing Clear/On
 * Does not set comm open flag meaning you want to use @ref USB_ForceClose
 *
 * @param mode Inherited from the Comm interface; needs to be 0x20
 * @return 0: successful; 5: timeout; 10: Clear/On abort
 */
extern int (*USB_Open)(short mode);

extern int (*USB_IsOpen)(void);

extern int (*USB_Close)(void);

extern int (*USB_ForceClose)(bool abortPending);

extern int (*USB_Read)(void *out, int sz, short *count) wo(1, 2) wo(3);

extern int (*USB_ReadSingle)(void *out) wo(1);

extern int (*USB_Peek)(int idx, uint8_t *out) wo(2);

extern int (*USB_PollRX)(void);

extern int (*USB_ClearRX)(void);

/**
 * Sends a number of bytes through the USB port, by putting them in the transmit buffer.
 *
 * @param buf pointer to buffer containing the bytes to transmit
 * @param count amount of bytes to transmit from buf
 */
extern int (*USB_Write)(const void *buf, int count) ro(1, 2);

extern int (*USB_WriteSingle)(uint8_t x);

extern int (*USB_WriteUnbuffered)(uint8_t x);

extern int (*USB_PollTX)(void);

extern int (*USB_ClearTX)(void);

#undef ro
#undef rw
#undef wo

#ifdef __cplusplus
}
#endif
#ifdef constexpr
#undef constexpr
#endif
