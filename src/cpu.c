#include "../include/cpu.h"

byte V[16];
word PC;
word SP;
word I;

void execute() {
    word opcode = 0;
    word opcode_first_nibble = opcode >> 12; 

    switch(opcode_first_nibble) {
        case 0x0: {
            if(opcode == 0x00E0) {

            }else if(opcode == 0x00EE) {

            }else {
                /* PC = (opcode << 4); */
            }
        }
        case 0x1: 
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:

        default: break;
    }
    printf("Hi from cpu\n");
}