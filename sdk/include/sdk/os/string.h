/**
 * @file
 * @brief String manipulation functions.
 * 
 * For documentation, see the C standard library.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define __UCONCAT(a, b) #a b
#define _UCONCAT(a, b) __UCONCAT(a, b)
#define UCONCAT(b) _UCONCAT(__USER_LABEL_PREFIX__, b)

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

#define inline inline __attribute__((always_inline))

extern char *(*String_Strcat)(char *dest, const char *src) cstr(1) cstr(2) rw(1) ro(2);

extern const char *(*_FP_String_Strchr)(const char *str, char c) __asm__ (UCONCAT("String_Strchr")) cstr(1) ro(1);

static inline __attribute__((pure)) cstr(1) ro(1) const char *String_Strchr(const char *str, char c) {
    return _FP_String_Strchr(str, c);
}

extern int (*_FP_String_Strcmp)(const char *str1, const char *str2) __asm__ (UCONCAT("String_Strcmp")) cstr(1) cstr(2) ro(1) ro(2);

static inline __attribute__((pure)) cstr(1) cstr(2) ro(1) ro(2) int String_Strcmp(const char *str1, const char *str2) {
    return _FP_String_Strcmp(str1, str2);
}

extern char *(*String_Strcpy)(char *destination, const char *source) cstr(2) wo(1) ro(2);

extern unsigned int (*_FP_String_Strlen)(const char *str) __asm__ (UCONCAT("String_Strlen")) cstr(1) ro(1);

static inline __attribute__((pure)) cstr(1) ro(1) unsigned int String_Strlen(const char *str) {
    return _FP_String_Strlen(str);
}

#undef cstr
#undef ro
#undef rw
#undef wo
#undef __UCONCAT
#undef _UCONCAT
#undef UCONCAT
#undef inline

#ifdef __cplusplus
}
#endif
