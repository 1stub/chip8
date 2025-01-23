#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "memory.h"

#define X       (opcode >> 8) & 0x000F
#define Y       (opcode >> 4) & 0x000F
#define N       (opcode & 0x000F)
#define NN      (opcode & 0x00FF)
#define NNN     (opcode & 0x0FFF)

extern byte V[16];
extern word PC;
extern word SP;
extern word I;

void cpu_init();
void execute();

#endif