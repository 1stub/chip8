#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

extern byte memory[CHIP8_MEMORY_SIZE];
extern bool pixel_buffer[CHIP8_HEIGHT][CHIP8_WIDTH];

byte read(word index);
void write();
void load_rom(char *file);

#endif