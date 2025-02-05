#include "../include/emulate.h"
#include "../include/cpu.h"

#include <SDL2/SDL.h>

#define INSTRUCTION_DELAY_MS 16

void emulate() {
    bool quit = false;
    Uint32 last_time = SDL_GetTicks();
    
    init_display();
    cpu_init();

    /* Every 16ms we execute NUM_INSTRS_PER_FRAME instrs */
    while(quit == false) {
        Uint32 current_time = SDL_GetTicks();
        Uint32 time_diff = current_time - last_time;

        if(time_diff >= INSTRUCTION_DELAY_MS) {
            int it;
            for(it = 0; it < NUM_INSTRS_PER_FRAME; it++) {
                execute(it);
                update_display(&quit);
            }
            last_time += INSTRUCTION_DELAY_MS;
            
            /* Timers update every NUM_INSTRS_PER_FRAME instructions, not every instruction */
            update_timers(); 
        }
    }
}
