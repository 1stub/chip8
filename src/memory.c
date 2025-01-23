#include "../include/memory.h"

byte memory[4096];

byte read() {
    return 0;
}

void write() {

}

void load_rom(char *file){
    FILE *fp;
    size_t ret;
    fp = fopen(file, "rb");
    if(!fp){
        perror("fopen - unable to open argv[1]");
        return ;
    }

    ret = fread(memory, sizeof(byte), 0x10000, fp);
    if (ret == 0) {
        fprintf(stderr, "fread() failed or file is empty\n");
    }
    if(!ret){
        fprintf(stderr, "fread() failed: %ld\n", ret);
    }

    fclose(fp);
}
