#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

extern byte memory[0x100];

byte read();
void write();

#endif