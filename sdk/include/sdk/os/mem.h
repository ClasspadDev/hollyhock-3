/**
 * @file
 * @brief Functions used for modifying and allocating memory.
 * 
 * Similar to the memory functions provided by the C standard library.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define __UCONCAT(a, b) #a b
#define _UCONCAT(a, b) __UCONCAT(a, b)
#define UCONCAT(b) _UCONCAT(__USER_LABEL_PREFIX__, b)

#include <stdint.h>
#include <stddef.h>

struct Mem_FreeListBlock {
    size_t size;
    struct Mem_FreeListBlock *next;
};

static void * const heap_start = (void *)0x8c280000;
static const
#ifdef __cplusplus
constexpr
#endif
size_t heap_size = 12 * 1024 * 1024; // 12MB
static void * const heap_end = (void *)0x8ce80000;

extern struct Mem_FreeListBlock ** const free_list_head;

/**
 * Frees memory allocated by @ref Mem_Malloc, allowing it to be reused.
 * 
 * @param ptr The pointer to the allocated region of memory to free.
 */
extern void (*Mem_Free)(void *ptr);

extern void *(*_FP_Mem_Malloc)(size_t size) __asm__ (UCONCAT("Mem_Malloc")) __attribute__((alloc_size(1), warn_unused_result));

/**
 * Allocates @p size bytes of memory.
 *
 * Invokes @ref Mem_Sbrk to enlarge the heap is necessary.
 *
 * @param size The number of bytes of memory to allocate.
 * @return A pointer to the allocated memory region.
 */
static void *Mem_Malloc(size_t size) __asm__ (UCONCAT("Mem_Malloc_Wrapper")) __attribute__((malloc, alloc_size(1), assume_aligned(4), unused, warn_unused_result));
void *Mem_Malloc(size_t size) {
    return _FP_Mem_Malloc(size);
}

/**
 * Copies one region of memory to another. Equivalent to the C standard library
 * function with the same name.
 *
 * Copies @p num bytes from @p source to @p destination.
 * 
 * @param[out] destination A pointer to the destination of the copy.
 * @param[in] source A pointer to the source for the copy.
 * @param num The number of bytes to copy.
 * @return @p destination
 */
extern void *(*Mem_Memcpy)(void *destination, const void *source, size_t num)
#ifndef __clang__
__attribute__((access(read_only, 2, 3), access(write_only, 1, 3)))
#endif
;

/**
 * Sets a region of memory to a specific value. Equivalent to the C standard
 * library function with the same name.
 * 
 * Fills the region pointed to by @p ptr with @p num bytes of value @p value
 * (zero-extended to a byte).
 *
 * @param[out] ptr A pointer to the region of memory to fill.
 * @param value The value to fill the memory region with.
 * @param num The number of bytes to fill.
 * @return @p ptr
 */
extern void *(*Mem_Memset)(void *ptr, int value, size_t num)
#ifndef __clang__
__attribute__((access(write_only, 1, 3)))
#endif
;

/**
 * Changes the heap boundary by @p size bytes.
 *
 * Be sure of what you are doing!
 *
 * @param size The number of bytes to change the heap boundary.
 * @return A pointer to the new heap boundary. Or -1 if OOM
 */
extern void *(*Mem_Sbrk)(long size);

/**
 * Changes what happens in a OOM situation.
 *
 * If 0, display a OOM message. If 1 @ref Mem_Sbrk returns -1.
 *
 * @param newMode The new mode or -1 to just query.
 * @return The old mode.
 */
extern signed char (*Mem_SbrkMode)(signed char newMode);

#undef __UCONCAT
#undef _UCONCAT
#undef UCONCAT

#ifdef __cplusplus
}
#endif