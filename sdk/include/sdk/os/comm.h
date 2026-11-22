/**
 * @file
 * @brief Status codes shared by the Comm, Serial and USB SDK interfaces.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Status returned by the Comm, Serial and USB SDK entry points.
 *
 * The OS returns this value as a 16-bit code in r0. All values below fit in
 * 16 bits; the names are shared across all three transports.
 */
enum Comm_Status {
    COMM_OK = 0x00,              /**< operation succeeded */
    COMM_NO_DATA = 0x01,         /**< no data yet / retry (IsOpen: open) */
    COMM_BUSY = 0x02,            /**< TX buffer full / no space / bad length */
    COMM_NOT_READY = 0x03,       /**< not configured; Serial_Open: bad config */
    COMM_ALREADY_OPEN = 0x04,    /**< port already open */
    COMM_NOT_OPEN = 0x05,        /**< session not open; open/close failed */
    COMM_EVENT = 0x06,           /**< event mask in the out-param */
    COMM_READY = 0x08,           /**< caller form of COMM_EVENT; IsOpen: configured */
    COMM_ERROR = 0x09,           /**< unknown / unmapped backend status */
    COMM_CANCELLED = 0x0a,       /**< user abort / timeout */
    COMM_NOT_INITIALISED = 0x0b, /**< USB API / transport not open */
    COMM_INVALID_ARG = 0x12,     /**< invalid serial mode bits */
    COMM_WOULD_BLOCK = 0x18      /**< no data available */
};

#ifdef __cplusplus
}
#endif
