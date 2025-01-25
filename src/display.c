#include "../include/display.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

SDL_Window* win = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event e;

void init_display() {
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

    memset(pixel_buffer, 0, sizeof(pixel_buffer));
}

static void render_pixel_buffer() {
    SDL_Rect pixel_rect;  /* Declare variables at the beginning */
    int x, y;

    pixel_rect.w = DISPLAY_SCALE;
    pixel_rect.h = DISPLAY_SCALE;

    /* Need to clear screen between each frame, could be optimized though */
    SDL_FillRect(win_surface, NULL, SDL_MapRGB(win_surface->format, 0x00, 0x00, 0x00));

    for ( y = 0; y < CHIP8_HEIGHT; y++) {
        for ( x = 0; x < CHIP8_WIDTH; x++) {
            if (pixel_buffer[y][x] != 0) {
                pixel_rect.x = x * DISPLAY_SCALE;  
                pixel_rect.y = y * DISPLAY_SCALE;
                SDL_FillRect(win_surface, &pixel_rect, SDL_MapRGB(win_surface->format, 0xFF, 0xDD, 0x33)); 
            }
        }
    }

    SDL_UpdateWindowSurface(win);  
}

void update_display(bool* quit) {
    static int count = 0;
    count++;
    
    while( SDL_PollEvent( &e ) ) { 
        switch(e.type) {
            case SDL_QUIT: {
                *quit = true;
            }
            break;
            case SDL_KEYDOWN: { 
                int i;
                for( i = 0; i < 16; i++) {
                    if(sdl_chip8_keymap[i] == e.key.keysym.sym) {
                        keyboard_status[i] = true;
                        just_pressed_key = true;
                    }
                }
            }
            break;

            case SDL_KEYUP: {
                int i;
                for( i = 0; i < 16; i++) {
                    if(sdl_chip8_keymap[i] == e.key.keysym.sym) {
                        keyboard_status[i] = false;
                        just_pressed_key = true;
                    }
                }
            }
            break;

            default: break;
        }
    }

    /* TODO: Need to properly sync this to 60hz, runs super fast now */
    /* I suspect we could just use SDL wait calls for this */
    if(count > 700) {
        SDL_ShowWindow(win);
        render_pixel_buffer();
        count = 0;
    }
}