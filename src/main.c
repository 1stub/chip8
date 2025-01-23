#include "../include/emulate.h"
#include "../include/memory.h"

int main(int argc, char **argv){
    load_rom(argv[1]);
    emulate();

    return 0;
}
