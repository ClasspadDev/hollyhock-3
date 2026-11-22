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
#include <sdk/os/comm.h>

static constexpr const uint16_t MODE_USB = 0x20;

/**
 * Opens the USB port.
 * Abortable by pressing Clear/On
 * Does not set comm open flag meaning you want to use @ref USB_ForceClose
 *
 * @param mode Inherited from the Comm interface; needs to be 0x20
 * @return COMM_OK, COMM_BUSY, COMM_NOT_READY or COMM_CANCELLED
 *         (see @ref Comm_Status)
 */
extern enum Comm_Status (*USB_Open)(uint16_t mode);

extern enum Comm_Status (*USB_IsOpen)(void);

extern enum Comm_Status (*USB_Close)(void);

extern enum Comm_Status (*USB_ForceClose)(uint16_t mode);

/**
 * Reads up to sz bytes into out and writes the number received to count.
 */
extern enum Comm_Status (*USB_Read)(void *out, uint16_t sz, uint16_t *count) wo(1) wo(3);

extern enum Comm_Status (*USB_ReadSingle)(void *out) wo(1);

extern enum Comm_Status (*USB_Peek)(uint16_t idx, void *out) wo(2);

/** @return number of bytes available to read. */
extern uint16_t (*USB_PollRX)(void);

extern enum Comm_Status (*USB_ClearRX)(void);

/**
 * Sends a number of bytes through the USB port, by putting them in the transmit buffer.
 *
 * @param buf pointer to buffer containing the bytes to transmit
 * @param count amount of bytes to transmit from buf
 */
extern enum Comm_Status (*USB_Write)(const void *buf, uint16_t count) ro(1);

extern enum Comm_Status (*USB_WriteSingle)(uint8_t x);

extern enum Comm_Status (*USB_WriteUnbuffered)(uint8_t x);

/** @return number of bytes of free space in the transmit buffer. */
extern uint16_t (*USB_PollTX)(void);

extern enum Comm_Status (*USB_ClearTX)(void);

#undef ro
#undef rw
#undef wo

#ifdef __cplusplus
}
#endif
#ifdef constexpr
#undef constexpr
#endif
