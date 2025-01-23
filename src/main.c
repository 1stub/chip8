#include "../include/emulate.h"
#include "../include/memory.h"

int main(int argc, char **argv){
    if(argc < 2) {
        fprintf(stderr, "Missing rom\n");
        return 0;
    }
    load_rom(argv[1]);
    emulate();

    return 0;
}
