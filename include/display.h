#ifndef DISPLAY_H
#define DISPLAY_H

#include "common.h"
#include <SDL2/SDL.h>

/* Scaled for large output, normally 64x32 */
#define DISPLAY_WIDTH 512
#define DISPLAY_HEIGHT 256

extern byte chip8_font[80];

void init_sdl();
void update_display(bool* quit);

#endif