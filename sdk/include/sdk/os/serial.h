/**
 * @file
 * @brief Functions used for sending and receiving serial data.
 */

// This works exactly the same as the calls on the cg50. Read more here:
// https://prizm.cemetech.net/index.php?title=Category:Syscalls:Serial
// These implementations are similar to:
// https://github.com/Jonimoose/libfxcg/blob/master/include/fxcg/serial.h

#pragma once

#ifdef __cplusplus
extern "C" {
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

enum Serial_Bitrate {
    SERIAL_BITRATE_300 = 0,
    SERIAL_BITRATE_1200 = 1,
    SERIAL_BITRATE_2400 = 2,
    SERIAL_BITRATE_4800 = 3,
    SERIAL_BITRATE_9600 = 5,
    SERIAL_BITRATE_19200 = 6,
    SERIAL_BITRATE_38400 = 7,
    SERIAL_BITRATE_57600 = 8,
    SERIAL_BITRATE_115200 = 9
};

enum Serial_Parity {
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_ODD = 1,
    SERIAL_PARITY_EVEN = 2
};

enum Serial_Data_Length {
    SERIAL_DATA_LENGTH_8BIT = 0,
    SERIAL_DATA_LENGTH_7BIT = 1
};

enum Serial_Stop_Bits {
    SERIAL_STOP_BITS_1 = 0,
    SERIAL_STOP_BITS_2 = 1
};

struct __attribute__((packed)) Serial_Settings {
    uint8_t zero;
    enum Serial_Bitrate bitrate : 8;
    enum Serial_Parity parity : 8;
    enum Serial_Data_Length data_length : 8;
    enum Serial_Stop_Bits stop_bits : 8;
    uint8_t zero2;
};

/**
 * Opens the 3-pin serial port with the specified parameters.
 * see https://prizm.cemetech.net/index.php?title=Serial_Open for more info.
 * mode:
 * 0 always 0
 * 1 bit rate	 0=300, 5=9600, 9=115200
 * 2 parity	 0=none, 1=odd, 2=even
 * 3 data length 0=8bit, 1=7bit
 * 4 stop bits	 0=1bit, 1=2bit
 * 5 always 0
 *
 * @param mode The mode of the serial port.
 * @return COMM_OK, COMM_ALREADY_OPEN or COMM_NOT_READY
 *         (see @ref Comm_Status)
 */
extern enum Comm_Status (*Serial_Open)(const struct Serial_Settings *mode) ro(1);

extern enum Comm_Status (*Serial_IsOpen)(void);

extern enum Comm_Status (*Serial_Close)(bool abortPending);

/**
 * Reads up to sz bytes into out and writes the number received to count.
 */
extern enum Comm_Status (*Serial_Read)(void *out, uint16_t sz, uint16_t *count) wo(1) wo(3);

extern enum Comm_Status (*Serial_ReadSingle)(void *out) wo(1);

extern enum Comm_Status (*Serial_Peek)(uint16_t idx, void *out) wo(2);

/** @return number of bytes available to read. */
extern uint16_t (*Serial_PollRX)(void);

extern enum Comm_Status (*Serial_ClearRX)(void);

/**
 * Sends a number of bytes through the 3-pin serial port, by putting them in the transmit buffer.
 * see https://prizm.cemetech.net/index.php?title=Serial_Write
 *
 * @param buf pointer to buffer containing the bytes to transmit
 * @param count amount of bytes to transmit from buf
 */
extern enum Comm_Status (*Serial_Write)(const void *buf, uint16_t count) ro(1);

extern enum Comm_Status (*Serial_WriteSingle)(uint8_t x);

extern enum Comm_Status (*Serial_WriteUnbuffered)(uint8_t x);

/** @return number of bytes of free space in the transmit buffer. */
extern uint16_t (*Serial_PollTX)(void);

extern enum Comm_Status (*Serial_ClearTX)(void);

#undef ro
#undef rw
#undef wo

#ifdef __cplusplus
}
#endif
