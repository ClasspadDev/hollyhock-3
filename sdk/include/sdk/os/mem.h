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

/**
 * Frees memory allocated by @ref malloc, allowing it to be reused.
 * 
 * @param ptr The pointer to the allocated region of memory to free.
 */
extern void (*Mem_Free)(void *ptr);

extern void *(*_FP_Mem_Malloc)(size_t size) __asm__ (UCONCAT("Mem_Malloc")) __attribute__((alloc_size(1), warn_unused_result));

/**
 * Allocates @p size bytes of memory.
 *
 * The current application crashes if the memory cannot be allocated.
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

#ifdef __cplusplus
}
#endif