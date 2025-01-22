#include "../include/emulate.h"

void emulate() {
    bool quit = false;
    init_sdl();

    while(quit == false) {
        execute();
        update_display(&quit);
    }
    printf("Hi from emulator\n");
}