/**
 * @file
 * @brief Functions for interacting with the calculator's file system.
 *
 * Exposes the Fugue atomic API, with byte-string paths, Fugue descriptors and
 * Fugue error codes. Directory searches belong to sdk/os/bfile.h and use a
 * different descriptor, path encoding and error namespace.
 * 
 * The storage which appears when the calculator is attached to a PC is found
 * under the path @c \\fls0\ (remember to escape the @c \ in strings!).
 *
 * Example: reading 256 bytes from a file called @c test.txt from the USB flash
 * @code{cpp}
 * int fd = File_Open("\\fls0\\test.txt", FILE_OPEN_READ);
 * if (fd < 0) {
 *     // An error occurred calling open
 *     goto exit;
 * }
 *
 * uint8_t buf[256];
 * int ret = File_Read(fd, buf, sizeof(buf));
 * if (ret < 0) {
 *     // An error occurred calling read
 *     File_Close(fd);
 *     goto exit;
 * }
 *
 * ret = File_Close(fd);
 * if (ret < 0) {
 *     // An error occurred calling close
 * }
 *
 * exit:
 * @endcode
 *
 * Example: writing 16 bytes to a non-existant file called @c f.bin in a folder @c test in the USB flash
 * @code{cpp}
 * int fd = File_Open("\\fls0\\test\\f.bin", FILE_OPEN_WRITE | FILE_OPEN_CREATE);
 * if (fd < 0) {
 *     // An error occurred calling open
 *     goto exit;
 * }
 *
 * uint8_t buf[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
 * int ret = File_Write(fd, buf, sizeof(buf));
 * if (ret < 0) {
 *     // An error occurred calling write
 *     File_Close(fd);
 *     goto exit;
 * }
 *
 * ret = File_Close(fd);
 * if (ret < 0) {
 *     // An error occurred calling close
 * }
 *
 * exit:
 * @endcode
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#else
#define constexpr
#endif

#include <stdint.h>
#include <stddef.h>

#ifndef __clang__
#define cstr(x) __attribute__((null_terminated_string_arg(x)))
#define ro(...) __attribute__((access(read_only, __VA_ARGS__)))
#define rw(...) __attribute__((access(read_write, __VA_ARGS__)))
#define wo(...) __attribute__((access(write_only, __VA_ARGS__)))
#else
#define cstr(x)
#define ro(...)
#define rw(...)
#define wo(...)
#endif

#define wu __attribute__((warn_unused_result))

/**
 * Errors returned by file system functions. All negative numbers.
 */
enum File_Error {
	FILE_OK = 0,
	FILE_ENOMEM = -1,
	FILE_EINVAL = -2,
	FILE_EDEVFAIL = -3,
	FILE_EMOUNTED = -4,
	FILE_EACCES = -5,
	FILE_EBADFSID = -6,
	FILE_ENOVOLUME = -7,
	FILE_ENOPATH = -8,
	FILE_EEXIST = -9,
	FILE_ENAMETOOLONG = -10,
	FILE_EOUTOFBOUND = -11,
	FILE_EUNFORMAT = -12,
	FILE_ENOSPC = -13,
	FILE_ENOENT = -14,
	FILE_EISDIRECTORY = -15,
	FILE_ESHARE = -16,
	FILE_EMFILE = -17,
	FILE_EBADF = -18,
	FILE_EEOF = -19,
	FILE_ENOTEMPTY = -20,
	FILE_ECLUSTERSIZEMISMATCH = -40,
	FILE_ESYSTEM = -99
};

/**
 * Values passed as the @c whence parameter to @ref File_Lseek.
 */
enum File_Whence {
	/// Set the file offset to @c offset.
	FILE_SEEK_SET = 0,
	/// Set the file offset to the current position, plus @c offset bytes.
	FILE_SEEK_CUR = 1,
	///Set the file offset to the end of the file, plus @c offset bytes.
	FILE_SEEK_END = 2
};

/**
 * Values passed as the @c flags parameter to @ref File_Open. Can be bitwise OR'd to
 * combine effects.
 */
enum File_Open {
	/// Open the file as readable.
	FILE_OPEN_READ = 1 << 0,
	/// Open the file as writable.
	FILE_OPEN_WRITE = 1 << 1,
	/// Create the file, if it does not already exist.
	FILE_OPEN_CREATE = 1 << 2,
	/// Opens the file with the file offset set to the end of the file.
	FILE_OPEN_APPEND = 1 << 4
};

/** FAT attribute bits returned in File_Stat.attributes. */
enum File_Attributes {
	FILE_ATTR_READ_ONLY = 0x01,
	FILE_ATTR_HIDDEN = 0x02,
	FILE_ATTR_SYSTEM = 0x04,
	FILE_ATTR_DIRECTORY = 0x10,
	FILE_ATTR_ARCHIVE = 0x20,
	/// Clear other status bits before passing a stat record to File_SetAttr.
	FILE_ATTR_SETTABLE_MASK = 0x37
};

/**
 * Retrieves the year from a @c struct @ref File_Stat date field.
 * 
 * @param date The date field from a @c struct @ref File_Stat.
 * @return The year encoded in the date field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatDateYear(uint16_t date) {
	return ((date >> 9) & 0x7F) + 1980;
}

/**
 * Retrieves the month from a @c struct @ref File_Stat date field.
 * 
 * @param date The date field from a @c struct @ref File_Stat.
 * @return The month encoded in the date field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatDateMonth(uint16_t date) {
	return (date >> 5) & 0xF;
}

/**
 * Retrieves the day from a @c struct @ref File_stat date field.
 * 
 * @param date The date field from a @c struct @ref File_stat.
 * @return The day encoded in the date field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatDateDay(uint16_t date) {
	return date & 0x1F;
}

/**
 * Retrieves the hour from a @c struct @ref File_Stat time field.
 * 
 * @param time The time field from a @c struct @ref File_Stat.
 * @return The hour encoded in the time field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatTimeHour(uint16_t time) {
	return (time >> 11) & 0x1F;
}

/**
 * Retrieves the minute from a @c struct @ref File_Stat time field.
 * 
 * @param time The time field from a @c struct @ref File_Stat.
 * @return The minute encoded in the time field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatTimeMinute(uint16_t time) {
	return (time >> 5) & 0x3F;
}

/**
 * Retrieves the second from a @c struct @ref File_Stat time field.
 * 
 * Has a maximum resolution of 2 seconds.
 * 
 * @param time The time field from a @c struct @ref File_Stat.
 * @return The second encoded in the time field.
 */
static inline __attribute__((pure)) uint16_t constexpr File_StatTimeSecond(uint16_t time) {
	return (time & 0x1F) * 2;
}

/**
 * Information about a file as retrieved from @ref File_Fstat or @ref File_Stat.
 * 
 * Dates and times are stored as a bitfield. The values they represent can be
 * accessed using the @ref File_StatDateYear, @ref File_StatDateMonth, @ref File_StatDateDay,
 * @ref File_StatTimeHour, @ref File_StatTimeMinute, and @ref File_StatTimeSecond helper
 * functions.
 *
 * Dates are stored in the following format:
 * <table>
 *     <tr>
 *         <td><b>Bit</b></td>
 *         <td><b>15</b></td>
 *         <td><b>14</b></td>
 *         <td><b>13</b></td>
 *         <td><b>12</b></td>
 *         <td><b>11</b></td>
 *         <td><b>10</b></td>
 *         <td><b>9</b></td>
 *         <td><b>8</b></td>
 *         <td><b>7</b></td>
 *         <td><b>6</b></td>
 *         <td><b>5</b></td>
 *         <td><b>4</b></td>
 *         <td><b>3</b></td>
 *         <td><b>2</b></td>
 *         <td><b>1</b></td>
 *         <td><b>0</b></td>
 *     </tr>
 *     <tr>
 *         <td><b>Value</b></td>
 *         <td colspan='7'>Year - 1980</td>
 *         <td colspan='4'>Month (indexed from 1)</td>
 *         <td colspan='5'>Day (indexed from 1)</td>
 *     </tr>
 * </table> 
 *
 * Times are stored in the following format:
 * <table>
 *     <tr>
 *         <td><b>Bit</b></td>
 *         <td><b>15</b></td>
 *         <td><b>14</b></td>
 *         <td><b>13</b></td>
 *         <td><b>12</b></td>
 *         <td><b>11</b></td>
 *         <td><b>10</b></td>
 *         <td><b>9</b></td>
 *         <td><b>8</b></td>
 *         <td><b>7</b></td>
 *         <td><b>6</b></td>
 *         <td><b>5</b></td>
 *         <td><b>4</b></td>
 *         <td><b>3</b></td>
 *         <td><b>2</b></td>
 *         <td><b>1</b></td>
 *         <td><b>0</b></td>
 *     </tr>
 *     <tr>
 *         <td><b>Value</b></td>
 *         <td colspan='5'>Hour</td>
 *         <td colspan='6'>Minute</td>
 *         <td colspan='5'>Second (divided by two, rounded down)</td>
 *     </tr>
 * </table>
 */
struct File_Stat {
	/// FAT attributes; bit 0x10 identifies a directory.
	uint32_t attributes;

	/**
	 * The size of the file, in bytes.
	 */
	uint32_t fileSize;

	/**
	 * The creation date of the file. For information on the storage format,
	 * see @c struct @ref stat.
	 */
	uint16_t creationDate;

	/**
	 * The creation time of the file. For information on the storage format,
	 * see @c struct @ref stat.
	 */
	uint16_t creationTime;

	/**
	 * The date the file was last modified. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastModifiedDate;

	/**
	 * The time the file was last modified. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastModifiedTime;

	uint16_t unknown2;

	/**
	 * The date the file was last accessed. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastAccessedDate;
};

/**
 * Capacity and geometry returned by File_FsInfo.
 *
 * The firmware writes a 0x34-byte, 4-byte-aligned record. The three byte counts
 * are 64-bit values in native byte order. Reserved words are output fields with
 * OS-specific meanings; callers should not interpret them.
 */
struct __attribute__((packed, aligned(4))) File_FsInfo {
	uint64_t totalBytes;
	uint64_t freeBytes;
	uint64_t usedBytes;
	uint32_t reserved0[3];
	uint32_t sectorSize;
	uint32_t sectorsPerCluster;
	uint32_t reserved1[2];
};

/**
 * Closes an open file.
 *
 * @param fd The file descriptor for the open file.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Close)(int fd);

/**
 * Retrieves information about an open file.
 *
 * @param fd The file descriptor of an open file to retrieve the information of.
 * @param[out] buf The retrieved information about the file.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Fstat)(int fd, struct File_Stat *buf) wu wo(2);

/**
 * Retrieves the memory address of a file.
 *
 * If the file is empty or the offset would point outside of the file,
 * @c FILE_EINVAL is returned.
 *
 * @param fd The file descriptor of an open file.
 * @param offset An offset to apply to the pointer to the file's data.
 * @param[out] addr The address of the file's data.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_GetAddr)(int fd, int offset, const void **addr) wu wo(3);

/**
 * Repositions the file offset of the file descriptor. The new position depends
 * on the value of both @p offset and @p whence. See @ref lseek_whence_values.
 *
 * @param fd The file descriptor of an open file to change the file offset of.
 * @param offset The new offset, relative to some point.
 * @param whence Where @p offset is relative to.
 * @return The new file offset on success, or a negative error code on failure.
 */
extern int (*File_Lseek)(int fd, int offset, enum File_Whence whence) wu;

/**
 * Creates a directory.
 *
 * @param[in] path The path to the directory to be created.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Mkdir)(const char *path) wu cstr(1) ro(1);

/**
 * Opens a file on the file system.
 * 
 * The argument @p flags is fed a bitwise OR'd combination of flags. See
 * @ref open_flags_values.
 *
 * @param[in] path The path to the file to open.
 * @param flags A bitfield describing the mode in which to open the file.
 * @return A file descriptor on success, or a negative error code on failure.
 */
extern int (*File_Open)(const char *path, int flags) wu cstr(1) ro(1);

/**
 * Reads up to @c count bytes from a file, and stores them in @c buf.
 *
 * If @c count bytes cannot be read from the file, as many bytes as possible are
 * read (i.e. between 0 and @c count - 1 bytes are read and stored in @c buf).
 *
 * @param fd The file descriptor of an open file to read from.
 * @param[out] buf A buffer to place the read bytes into.
 * @param count The maximum number of bytes to read.
 * @return The number of bytes read on success, or a negative error code on
 * failure.
 */
extern int (*File_Read)(int fd, void *buf, int count) wu wo(2, 3);

/**
 * Deletes a file or directory.
 *
 * @param[in] path The path to the file or directory to be deleted.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Remove)(const char *path) cstr(1) ro(1);

/**
 * Renames a file or directory.
 *
 * @param[in] oldPath The path to the file or directory to be renamed.
 * @param[in] newPath The path to the new name for the file or directory.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Rename)(const char *oldPath, const char *newPath) wu cstr(1) cstr(2) ro(1) ro(2);

/**
 * Retrieves information about a file.
 *
 * @param[in] path The path to the file to retrieve the information of.
 * @param[out] buf The retrieved information about the file.
 * @return 0 on success, or a negative error code on failure.
 */
extern enum File_Error (*File_Stat)(const char *path, struct File_Stat *buf) wu cstr(1) ro(1) wo(2);

/**
 * Writes @c count bytes from @c buf to a file.
 *
 * @param fd The file descriptor of an open file to write to.
 * @param[in] buf A buffer of bytes to write to the file.
 * @param count The number of bytes from @c buf to write to the file.
 * @return The number of bytes written on success, or a negative error code on
 * failure.
 */
extern int (*File_Write)(int fd, const void *buf, int count) wu ro(2, 3);

extern enum File_Error (*File_Flush)(int fd) wu;

/**
 * Query a mounted device's capacity and geometry.
 * @param device Device root without a trailing separator, eg. "\\fls0".
 * @param[out] info Capacity and geometry record.
 * @return 0 on success, or a Fugue error.
 */
extern enum File_Error (*File_FsInfo)(const char *device, struct File_FsInfo *info) wu cstr(1) ro(1) wo(2);

/**
 * Update FAT attributes and timestamps from a File_Stat record.
 *
 * Obtain the record with File_Stat first so existing timestamps are preserved.
 * Mask attributes with FILE_ATTR_SETTABLE_MASK and preserve the directory bit:
 * this call cannot change a file into a directory or vice versa. fileSize is
 * ignored; this operation does not resize the file.
 * @return 0 on success, or a Fugue error.
 */
extern enum File_Error (*File_SetAttr)(const char *path, const struct File_Stat *info) wu cstr(1) ro(1) ro(2);

/**
 * Run the mounted device's reclaim/garbage-collection operation.
 * @param device Device root without a trailing separator, eg. "\\fls0".
 * @param workLimit Nonnegative OS-specific work limit; its unit is not known.
 * @return A nonnegative device-specific result, or a Fugue error.
 */
extern int (*File_Reclaim)(const char *device, int workLimit) wu cstr(1) ro(1);

/**
 * Repair a mounted filesystem's allocation metadata.
 *
 * This is a potentially long-running, metadata-changing maintenance operation.
 * The filesystem must not be in use; busy states return FILE_EACCES.
 * @param device Device root without a trailing separator, eg. "\\fls0".
 * @return 0 on success, or a Fugue error.
 */
extern enum File_Error (*File_Salvage)(const char *device) wu cstr(1) ro(1);

#undef cstr
#undef ro
#undef rw
#undef wo
#undef wu

#ifdef __cplusplus
}
#endif
#ifdef constexpr
#undef constexpr
#endif
