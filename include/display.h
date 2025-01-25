#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include "memory.h"
#include "cpu.h"
#include <SDL2/SDL.h>

void init_display();
void update_display(bool* quit);

#endif