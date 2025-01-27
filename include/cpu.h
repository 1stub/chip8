#ifndef CPU_H
#define CPU_H

#include "common.h"
#include "memory.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define X       (opcode >> 8) & 0x000F
#define Y       (opcode >> 4) & 0x000F
#define N       (opcode & 0x000F)
#define NN      (opcode & 0x00FF)
#define NNN     (opcode & 0x0FFF)

extern byte V[16];
extern SDL_Keycode chip8KeyMap[16];
extern word PC;
extern word SP;
extern word I;
extern byte delay_timer;
extern byte sound_timer;
extern bool keyboard_status[CHIP8_KEY_COUNT];
extern bool can_render;

void cpu_init();
void execute();
void update_timers();

#endif