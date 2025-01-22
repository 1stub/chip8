#ifndef CPU_H
#define CPU_H

#include "common.h"

#define X       (opcode & 0x0F00) >> 8
#define Y       (opcode & 0x00F0) >> 4
#define N       (opcode & 0x000F)
#define NN      (opcode & 0x00FF)
#define NNN     (opcode & 0x0FFF)

extern byte V[16];

void execute();

#endif