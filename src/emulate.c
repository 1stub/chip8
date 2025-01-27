#include "../include/emulate.h"
#include "../include/cpu.h"

#include <SDL2/SDL.h>

#define INSTRUCTION_DELAY_MS 16
#define NUM_INSTRS_PER_FRAME 8

void emulate() {
    bool quit = false;
    Uint32 last_time = SDL_GetTicks();
    
    init_display();
    cpu_init();

    /* Every 16ms we execute 8 instrs */
    while(quit == false) {
        Uint32 current_time = SDL_GetTicks();
        Uint32 time_diff = current_time - last_time;

        if(time_diff >= INSTRUCTION_DELAY_MS) {
            int it;
            for(it = 0; it < NUM_INSTRS_PER_FRAME; it++) {
                execute();
                update_timers();
            }
            update_display(&quit);

            debug_print("[DEBUG] Time of render %f\n", last_time);
            last_time += INSTRUCTION_DELAY_MS;
        }
    }

    printf("Hi from emulator\n");
}