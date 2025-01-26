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

/**
* To fix flickering we can interpolate when pixels are turned off.
* Just store a temporary copy of the current buffer, look for those
* pixels that have changed between the prev and new frames, make those
* a greyish color for one frame then make zero.
**/
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

/* Had to reference other emulator for this... bruh */
int keymap(unsigned char k) {
    switch (k) {
        case '1': return 0x1;
        case '2': return 0x2;
        case '3': return 0x3;
        case '4': return 0xc;

        case 'q': return 0x4;
        case 'w': return 0x5;
        case 'e': return 0x6;
        case 'r': return 0xd;

        case 'a': return 0x7;
        case 's': return 0x8;
        case 'd': return 0x9;
        case 'f': return 0xe;

        case 'z': return 0xa;
        case 'x': return 0x0;
        case 'c': return 0xb;
        case 'v': return 0xf;

        default:  return -1;  
    }
}

void update_display(bool* quit) {
    static Uint32 last_frame_time = 0;
    Uint32 current_time = SDL_GetTicks();
    
    while( SDL_PollEvent( &e ) ) { 
        switch(e.type) {
            case SDL_QUIT: {
                *quit = true;
            }
            break;
            case SDL_KEYDOWN: { 
                int chip8_key = keymap(e.key.keysym.sym);
                if (chip8_key != -1) {  
                    keyboard_status[chip8_key] = true;
                }
            }
            break;

            case SDL_KEYUP: {
                int chip8_key = keymap(e.key.keysym.sym);
                if (chip8_key != -1) {  
                    keyboard_status[chip8_key] = false;
                }
            }
            break;

            default: break;
        }
    }

    if(current_time - last_frame_time >= FRAME_DELAY) {
        SDL_ShowWindow(win);
        render_pixel_buffer();

        last_frame_time = current_time;
    } else {
        SDL_Delay(FRAME_DELAY - (current_time - last_frame_time));
    }
}