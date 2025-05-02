#pragma once

#ifdef __cplusplus
extern "C" {
#else 
#define constexpr
#endif

#include <stdint.h>

//Graphics stuff

static uint16_t * const vram = (uint16_t *)0x8c000000; //address of the vram
static const constexpr unsigned int width = 320; //width of the screen
static const constexpr unsigned int height = 528; //height of the screen

void line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint16_t color);
void vline(unsigned int x, unsigned int y1, unsigned int y2, uint16_t color);
void triangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint16_t colorFill, uint16_t colorLine);
void fillScreen(uint16_t color);

static inline __attribute__((pure)) uint16_t constexpr color(uint8_t R, uint8_t G, uint8_t B) {
    return	(((R<<8) & 0xF800) |
             ((G<<3) & 0x07E0) |
             ((B>>3) & 0x001A));
}

static inline void setPixel(unsigned int x, unsigned int y, uint32_t color) {
    if(x < width && y < height)
        vram[width*y + x] = color;
}

//Stuff for Initialisation and stuff

void calcInit(void);
void calcExit(void);

//Stuff for the keyboard

#ifndef __clang__
__attribute__((access(write_only, 1), access(write_only, 2)))
#endif
void getKey(uint32_t *key1, uint32_t *key2);

enum Keys1 {
    KEY_SHIFT       = (int)0x80000000,
    KEY_CLEAR       = 0x00020000, //The Power key
    KEY_BACKSPACE   = 0x00000080,
    KEY_LEFT        = 0x00004000,
    KEY_RIGHT       = 0x00008000,
    KEY_Z           = 0x00002000,
    KEY_POWER       = 0x00000040, //The exponent key
    KEY_DIVIDE      = 0x40000000,
    KEY_MULTIPLY    = 0x20000000,
    KEY_SUBTRACT    = 0x10000000,
    KEY_ADD	        = 0x08000000,
    KEY_EXE	        = 0x04000000,
    KEY_EXP	        = 0x00000004,
    KEY_3           = 0x00000008,
    KEY_6           = 0x00000010,
    KEY_9           = 0x00000020,
};

enum Keys2 {
    KEY_KEYBOARD        = (int)0x80000000,
    KEY_UP              = 0x00800000,
    KEY_DOWN            = 0x00400000,
    KEY_EQUALS          = 0x00000080,
    KEY_X               = 0x00000040,
    KEY_Y               = 0x40000000,
    KEY_LEFT_BRACKET    = 0x00000020,
    KEY_RIGHT_BRACKET   = 0x00000010,
    KEY_COMMA           = 0x00000008,
    KEY_NEGATIVE        = 0x00000004,
    KEY_0               = 0x04000000,
    KEY_DOT             = 0x00040000,
    KEY_1               = 0x08000000,
    KEY_2               = 0x00080000,
    KEY_4               = 0x10000000,
    KEY_5               = 0x00100000,
    KEY_7               = 0x20000000,
    KEY_8               = 0x00200000,
};

#ifdef __cplusplus
}

static inline
#ifndef __clang__
__attribute__((access(write_only, 1), access(write_only, 2)))
#endif
void getKey(Keys1 *key1, Keys2 *key2) {
    uint32_t k1, k2;
    getKey(&k1, &k2);
    *key1 = static_cast<Keys1>(k1);
    *key2 = static_cast<Keys2>(k2);
}

static inline __attribute__((pure)) bool constexpr testKey(uint32_t key1, uint32_t, Keys1 key) {
    return key1 & key;
}

static inline __attribute__((pure)) bool constexpr testKey(uint32_t, uint32_t key2, Keys2 key) {
    return key2 & key;
}
#else
#undef constexpr
#endif
