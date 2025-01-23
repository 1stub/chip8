#include "../include/cpu.h"

#include <string.h>

byte chip8_font[80] = {
   0xF0,  0x90,  0x90,  0x90,  0xF0, /* 0 */
   0x20,  0x60,  0x20,  0x20,  0x70, /* 1 */
   0xF0,  0x10,  0xF0,  0x80,  0xF0, /* 2 */
   0xF0,  0x10,  0xF0,  0x10,  0xF0, /* 3 */
   0x90,  0x90,  0xF0,  0x10,  0x10, /* 4 */
   0xF0,  0x80,  0xF0,  0x10,  0xF0, /* 5 */
   0xF0,  0x80,  0xF0,  0x90,  0xF0, /* 6 */
   0xF0,  0x10,  0x20,  0x40,  0x40, /* 7 */
   0xF0,  0x90,  0xF0,  0x90,  0xF0, /* 8 */
   0xF0,  0x90,  0xF0,  0x10,  0xF0, /* 9 */
   0xF0,  0x90,  0xF0,  0x90,  0x90, /* A */
   0xE0,  0x90,  0xE0,  0x90,  0xE0, /* B */
   0xF0,  0x80,  0x80,  0x80,  0xF0, /* C */
   0xE0,  0x90,  0x90,  0x90,  0xE0, /* D */
   0xF0,  0x80,  0xF0,  0x80,  0xF0, /* E */
   0xF0,  0x80,  0xF0,  0x80,  0x80  /* F */
};

byte V[16];
word PC;
word SP;
word I;

void cpu_init() {
    int i;
    PC = CHIP8_ROM_BP;
    SP = 0x0000;
    I  = 0x0000;

    for(i = 0; i < 80; i++) {
        memory[i] = chip8_font[i];
    }

    memset(V, 0, sizeof(byte) * 16);
}

void execute() {
    word opcode = read(PC) << 8 | read(PC + 1);
    word opcode_first_nibble = opcode >> 12; 

    printf("OPCODE %x\n", opcode);

    switch(opcode_first_nibble) {
        case 0x0: {
            if(opcode == 0x00E0) {
                memset(pixel_buffer, 0, sizeof(pixel_buffer));
            }else if(opcode == 0x00EE) {

            }else {
                PC = NNN;
            }
            PC += 2;
            break;
        }
        case 0x1: {
            PC = NNN;
            break;
        }
        case 0x2: {
            PC += 2;
            break;
        }
        case 0x3: {
            PC += 2;
            break;
        }
        case 0x4: {
            PC += 2;
            break;
        }
        case 0x5: {
            PC += 2;
            break;
        }
        case 0x6: {
            V[X] = NN;
            PC += 2;
            break;
        }
        case 0x7: {
            V[X] += NN;
            PC += 2;
            break;
        }
        case 0x8: {
            PC += 2;
            break;
        }
        case 0x9: {
            PC += 2;
            break;
        }
        case 0xA: {
            I = NNN;
            PC += 2;
            break;
        }
        case 0xB: {
            PC += 2;
            break;
        }
        case 0xC: {
            PC += 2;
            break;
        }
        case 0xD: {
            int idx, pixel;

            V[0xF] = 0;
            for (idx = 0; idx < N; idx++) {
                byte data = read(I + idx);
                for (pixel = 0; pixel < 8; pixel++) {
                    byte sprite_pixel = GET_BIT(data, pixel);
                    bool* display_pixel = &pixel_buffer[(V[Y] + idx) % CHIP8_HEIGHT]
                                                      [(V[X] + (7 - pixel)) % CHIP8_WIDTH];

                    if(sprite_pixel != 0 && *display_pixel != 0){
                            V[0xF] = 1;
                    }
                    *display_pixel ^= sprite_pixel; /* XOR draw */
                }
            }
            PC += 2;
            break;
        }
        case 0xE: {
            PC += 2;
            break;
        }
        case 0xF: {
            PC += 2;
            break;
        }

        default: {
            break;
        }
    }
}