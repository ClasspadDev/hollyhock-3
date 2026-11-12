/**
 * @file
 * @brief Legacy BFile source-dispatching filesystem API.
 *
 * BFile uses 16-bit Casio FONTCHARACTER paths such as u"\\\\fls0\\*".
 * Its descriptors and errors are distinct from the Fugue atomic File_* API
 * in sdk/os/file.h. In particular, search handles must be closed here.
 *
 * The extra backend arguments on Open, Remove and Rename are part of the
 * CP-400 ABI; pass zero for storage memory. Create's backend argument is
 * forwarded to the selected device, but storage-memory creation ignores it.
 */
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __clang__
#define cstr(x) __attribute__((null_terminated_string_arg(x)))
#define ro(...) __attribute__((access(read_only, __VA_ARGS__)))
#define wo(...) __attribute__((access(write_only, __VA_ARGS__)))
#else
#define cstr(x)
#define ro(...)
#define wo(...)
#endif

#define wu __attribute__((warn_unused_result))

#define __UCONCAT(a, b) #a b
#define _UCONCAT(a, b) __UCONCAT(a, b)
#define UCONCAT(b) _UCONCAT(__USER_LABEL_PREFIX__, b)

typedef uint16_t char_const16_t;

enum BFile_Error {
	BFILE_OK = 0,
	BFILE_ENTRY_NOT_FOUND = -1,
	BFILE_ILLEGAL_PARAM = -2,
	BFILE_ILLEGAL_PATH = -3,
	BFILE_DEVICE_FULL = -4,
	BFILE_ILLEGAL_DEVICE = -5,
	BFILE_ILLEGAL_FILESYSTEM = -6,
	BFILE_ILLEGAL_SYSTEM = -7,
	BFILE_ACCESS_DENIED = -8,
	BFILE_ALREADY_LOCKED = -9,
	BFILE_ILLEGAL_TASK_ID = -10,
	BFILE_PERMISSION_ERROR = -11,
	BFILE_ENTRY_FULL = -12,
	BFILE_ALREADY_EXISTS = -13,
	BFILE_READ_ONLY_FILE = -14,
	BFILE_ILLEGAL_FILTER = -15,
	BFILE_ENUMERATE_END = -16,
	BFILE_DEVICE_CHANGED = -17,
	BFILE_ILLEGAL_SEEK_POS = -19,
	BFILE_ILLEGAL_BLOCK_FILE = -20,
	BFILE_NOT_MOUNT_DEVICE = -23,
	BFILE_NOT_UNMOUNT_DEVICE = -24,
	BFILE_CANNOT_LOCK_SYSTEM = -25,
	BFILE_RECORD_NOT_FOUND = -26,
	BFILE_NO_ALARM_SUPPORT = -28,
	BFILE_CANNOT_ADD_ALARM = -29,
	BFILE_FILE_FIND_USED = -30,
	BFILE_DEVICE_ERROR = -31,
	BFILE_SYSTEM_NOT_LOCKED = -32,
	BFILE_DEVICE_NOT_FOUND = -33,
	BFILE_FILE_TYPE_MISMATCH = -34,
	BFILE_NOT_EMPTY = -35,
	BFILE_BROKEN_SYSTEM_DATA = -36,
	BFILE_MEDIA_NOT_READY = -37,
	BFILE_TOO_MANY_ALARMS = -38,
	BFILE_SAME_ALARM_EXISTS = -39,
	BFILE_ACCESS_SWAP_AREA = -40,
	BFILE_MULTIMEDIA_CARD = -41,
	BFILE_COPY_PROTECTION = -42,
	BFILE_ILLEGAL_FILE_DATA = -43
};

enum BFile_EntryType {
	BFILE_FILE = 1,
	BFILE_DIRECTORY = 5
};

enum BFile_OpenMode {
	BFILE_READ_ONLY = 0x01,
	BFILE_WRITE_ONLY = 0x02,
	BFILE_READ_WRITE = 0x03,
	BFILE_SHARE = 0x80
};

/** Legacy attribute bits; these differ from File_Stat's FAT attribute bits. */
enum BFile_Attributes {
	BFILE_ATTR_READ_ONLY = 0x01,
	BFILE_ATTR_HIDDEN = 0x08
};

/** CP-400 metadata layout, distinct from File_Stat and other models' BFile info. */
struct BFile_FindInfo {
	uint32_t length;       ///< Record size, 0x14 bytes.
	uint16_t type;         ///< BFILE_FILE or BFILE_DIRECTORY.
	uint16_t attributes;
	uint32_t size;         ///< File size in bytes; zero for directories.
	uint8_t reserved[8];
};

/** Descriptor/value returns use signed int so negative BFile errors fit. */
extern int (*BFile_Open)(const char_const16_t *path, int mode, int backendArg) wu cstr(1) ro(1);
extern enum BFile_Error (*BFile_Close)(int fd);
extern int (*BFile_Size)(int fd) wu;
extern int (*BFile_Read)(int fd, void *data, int size, int position) wu wo(2, 3);
extern int (*BFile_Write)(int fd, const void *data, int size) wu ro(2, 3);
/**
 * Write at an absolute position, or at the current position if position < 0.
 * The file position advances. Return semantics follow the selected backend,
 * as with BFile_Write; this is not POSIX pwrite's position-preserving contract.
 */
extern int (*BFile_WriteSeek)(int fd, const void *data, int size, int position) wu ro(2, 3);
/** Seek return values depend on the backend and can include positive counts. */
extern int (*BFile_Seek)(int fd, int offset) wu;
extern enum BFile_Error (*BFile_Flush)(int fd) wu;
extern enum BFile_Error (*BFile_Remove)(const char_const16_t *path, int backendArg) cstr(1) ro(1);
extern enum BFile_Error (*BFile_Rename)(const char_const16_t *oldPath, const char_const16_t *newPath, int backendArg) wu cstr(1) cstr(2) ro(1) ro(2);
extern enum BFile_Error (*BFile_Create)(const char_const16_t *path, int type, void *backendArg) wu cstr(1) ro(1);

/** Query a FONTCHARACTER path without allocating a search handle. */
extern enum BFile_Error (*BFile_GetFileInfo)(const char_const16_t *path, struct BFile_FindInfo *info) wu cstr(1) ro(1) wo(2);
/**
 * Query free bytes. device is a FONTCHARACTER device name such as u"fls0",
 * rather than a file path. This legacy query exposes a 32-bit byte count;
 * File_FsInfo provides the full 64-bit capacity counters.
 */
extern enum BFile_Error (*BFile_GetFreeSpace)(const char_const16_t *device, uint32_t *freeBytes) wu cstr(1) ro(1) wo(2);
/**
 * Apply legacy attributes; pass zero for backendArg on storage memory.
 * The SMEM backend sets requested read-only/hidden bits without clearing
 * existing ones. Use File_SetAttr for full FAT attribute/timestamp updates.
 */
extern enum BFile_Error (*BFile_SetAttr)(const char_const16_t *path, uint32_t attributes, int backendArg) wu cstr(1) ro(1);
/**
 * Obtain a read-only mapped address for a storage-memory descriptor.
 * offset < 0 selects the current file position. On success, stores the address
 * and returns bytes remaining in its 4KiB mapped block, not bytes until EOF.
 * The returned extent is not a mapping of the whole file. Errors are BFile errors.
 */
extern int (*BFile_GetMappedAddress)(int fd, int offset, const void **address) wu wo(3);

/** Search a FONTCHARACTER pattern. Returns zero, or a BFile_Error. */
extern enum BFile_Error (*BFile_FindFirst)(const char_const16_t *pattern, int *findHandle,
	char_const16_t *name, struct BFile_FindInfo *info) __asm__(UCONCAT("File_FindFirst")) wu cstr(1) ro(1) wo(2) wo(3) wo(4);
/** Continue a search; BFILE_ENUMERATE_END signals exhaustion. */
extern enum BFile_Error (*BFile_FindNext)(int findHandle, char_const16_t *name, struct BFile_FindInfo *info) __asm__(UCONCAT("File_FindNext")) wu wo(2) wo(3);
extern enum BFile_Error (*BFile_FindClose)(int findHandle) __asm__(UCONCAT("File_FindClose"));

#undef cstr
#undef ro
#undef wo
#undef wu

#undef __UCONCAT
#undef _UCONCAT
#undef UCONCAT

#ifdef __cplusplus
}
#endif
