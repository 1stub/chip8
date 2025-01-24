#include "../include/memory.h"

#include "string.h"

byte memory[CHIP8_MEMORY_SIZE];
bool pixel_buffer[CHIP8_HEIGHT][CHIP8_WIDTH];

byte read(word index) {
    return memory[index];
}

void write(word address, byte value) {
    memory[address] = value;
}

void load_rom(char *file){

    FILE *fp;
    size_t ret;
    fp = fopen(file, "rb");
    if(!fp){
        perror("fopen - unable to open argv[1]");
        return ;
    }

    memset(memory, 0, sizeof(memory));

    ret = fread(&memory[CHIP8_ROM_BP], sizeof(byte), CHIP8_MEMORY_SIZE - CHIP8_ROM_BP, fp);
    if (ret == 0) {
        fprintf(stderr, "fread() failed or file is empty\n");
    }
    if(!ret){
        fprintf(stderr, "fread() failed: %ld\n", ret);
    }

    fclose(fp);
}
