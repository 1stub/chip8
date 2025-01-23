#include "../include/emulate.h"
#include "../include/cpu.h"

void emulate() {
    bool quit = false;
    init_display();
    cpu_init();

    while(quit == false) {
        execute();
        update_display(&quit);
    }
    printf("Hi from emulator\n");
}