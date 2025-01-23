#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

extern byte memory[4096];

byte read();
void write();
void load_rom(char *file);

#endif