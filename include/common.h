#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef unsigned char byte;
typedef unsigned short word;

#define true 1
#define false 0
#define bool int

#define GET_BIT(src, bit_no) (((src) & (1 << (bit_no))) >> (bit_no))

/* Scaled for large output, normally 64x32 */
#define DISPLAY_WIDTH 512
#define DISPLAY_HEIGHT 256

#define DISPLAY_SCALE DISPLAY_HEIGHT / CHIP8_HEIGHT 

#define CHIP8_WIDTH 64
#define CHIP8_HEIGHT 32

#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_ROM_BP 512

#define CHIP8_KEY_COUNT 16

#endif