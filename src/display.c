#include "../include/display.h"
#include <SDL2/SDL_video.h>

byte chip8_font[80] = {
   0xF0,  0x90,  0x90,  0x90,  0xF0, /* 0 */
   0x20,  0x60,  0x20,  0x20,  0x70, /* 1 */
   0xF0,  0x10,  0xF0,  0x80,  0xF0, /* 2 */
   0xF0,  0x10,  0xF0,  0x10,  0xF0, /* 3 */
   0x90,  0x90,  0xF0,  0x10,  0x10, /* 4 */
   0xF0,  0x80,  0xF0,  0x10,  0xF0, /* 5 */
   0xF0,  0x80,  0xF0,  0x90,  0xF0, /* 6 */
   0xF0,  0x10,  0x20,  0x40,  0x40, /* 7 */
   0xF0,  0x90,  0xF0,  0x90,  0xF0, /* 8 */
   0xF0,  0x90,  0xF0,  0x10,  0xF0, /* 9 */
   0xF0,  0x90,  0xF0,  0x90,  0x90, /* A */
   0xE0,  0x90,  0xE0,  0x90,  0xE0, /* B */
   0xF0,  0x80,  0x80,  0x80,  0xF0, /* C */
   0xE0,  0x90,  0x90,  0x90,  0xE0, /* D */
   0xF0,  0x80,  0xF0,  0x80,  0xF0, /* E */
   0xF0,  0x80,  0xF0,  0x80,  0x80  /* F */
};

SDL_Window* win = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event e;

void init_sdl() {
    if (SDL_Init(SDL_INIT_TIMER) == 0) {
        printf("[INFO] SDL Timer subsystem initialized successfully.\n");
    } else {
        fprintf(stderr, "[ERROR] SDL Timer initialization failed: %s\n", SDL_GetError());
    }

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        printf("[INFO] SDL Video subsystem initialized successfully.\n");
    } else {
        fprintf(stderr, "[ERROR] SDL Video initialization failed: %s\n", SDL_GetError());
    }

    if (SDL_Init(SDL_INIT_EVENTS) == 0) {
        printf("[INFO] SDL Events subsystem initialized successfully.\n");
    } else {
        fprintf(stderr, "[ERROR] SDL Events initialization failed: %s\n", SDL_GetError());
    }

    /* Having issues with initializing SDL_INIT_AUDIO, ignoring for now */

    win = SDL_CreateWindow(
        "Chip 8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_WINDOW_HIDDEN);

    if(win == NULL) {
        fprintf(stderr, "[ERROR] Unable to initialize SDL window!\n");
        return ;
    }

    win_surface = SDL_GetWindowSurface(win);
}

void update_display(bool* quit) {
    /* Filling rect will eventually just be done with data from cpu */
    SDL_FillRect( win_surface, NULL, SDL_MapRGB( win_surface->format, 0x00, 0x00, 0x00 ) );
    SDL_UpdateWindowSurface( win);

    SDL_ShowWindow(win);
    while( SDL_PollEvent( &e ) ) { 
        if( e.type == SDL_QUIT ) {
            *quit = true;
        }
    }
}