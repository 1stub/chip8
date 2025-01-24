#include "../include/cpu.h"

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
word stack[16];
word PC;
word SP;
word I;
byte delay_timer;
byte sound_timer;

void cpu_init() {
    int i;
    PC = CHIP8_ROM_BP;
    SP = 0x0000;
    I  = 0x0000;
    delay_timer = 0;
    sound_timer = 0;

    for(i = 0; i < 80; i++) {
        memory[i] = chip8_font[i];
    }

    memset(V, 0, sizeof(byte) * 16);
    memset(stack, 0, sizeof(word) * 16);
}

/** 
* For instructions that I wrote are ambiguous, this means their functionality
* differ depending on the interpreter used. For now I have picked the most
* common functionality, but this may change if I decide to really make this
* emulator work well.
**/
void execute() {
    word opcode = read(PC) << 8 | read(PC + 1);
    word opcode_first_nibble = opcode >> 12; 

    printf("OPCODE %x\n", opcode);

    switch(opcode_first_nibble) {
        case 0x0: {
            if(opcode == 0x00E0) {
                memset(pixel_buffer, 0, sizeof(pixel_buffer));
                PC += 2;
            }else if(opcode == 0x00EE) {
                if(SP == 0) {
                    PC = stack[SP];
                }
                else {
                    PC = stack[--SP];
                }
                PC += 2;
            }else {
                /* PC = NNN; */
                PC += 2;
            }
            break;
        }
        case 0x1: {
            PC = NNN;
            break;
        }
        case 0x2: {
            stack[SP++] = PC;
            PC = NNN;
            break;
        }
        case 0x3: {
            if(V[X] == NN) PC+=2;
            PC += 2;
            break;
        }
        case 0x4: {
            if(V[X] != NN) PC+=2;
            PC += 2;
            break;
        }
        case 0x5: {            
            if(V[X] == V[Y]) PC+=2;
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
            switch(N) {
                case   0: V[X] = V[Y]; break;
                case   1: V[X] |= V[Y]; break;
                case   2: V[X] &= V[Y]; break;
                case   3: V[X] ^= V[Y]; break;
                case   4: {
                    int sum = V[X] += V[Y]; 
                    if (sum > 255) V[0xF] = 1;
                    V[X] = sum;
                } 
                break;
                case   5: V[X] -= V[Y]; break;
                case   6: {
                    /* Some ambiguities to this instr */
                    word flag = V[X] & 0x0001;
                    V[X] >>= 1;
                    V[0xF] = flag;
                } break;
                case   7: V[X] = V[Y] - V[X]; break;
                case 0xE: { 
                    /* Some ambiguities to this instr */
                    word flag = V[X] & 0x0001;
                    V[X] = V[X] << 1;
                    V[0xF] = flag;
                }
                break; 
                default: break;
            }
            PC += 2;
            break;
        }
        case 0x9: {
            if(V[X] != V[Y]) PC+=2;
            PC += 2;
            break;
        }
        case 0xA: {
            I = NNN;
            PC += 2;
            break;
        }
        case 0xB: {
            /* Some ambiguities to this instr */
            PC = NNN + V[0];
            break;
        }
        case 0xC: {
            /* Maybe don't use rand() */
            byte r;
            srand(time(NULL));
            r = (rand() % 256) & NN;
            V[X] = r;
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
            switch(NN) {
                case 0x07: V[X] = delay_timer; break;
                case 0x15: delay_timer = V[X]; break;
                case 0x18: sound_timer = V[X]; break;
                case 0x1E: {
                    word sum = I + V[X];
                    if(sum >= 0x1000) V[0xF] = 1;
                    I = sum;
                }
                break;
                case 0x0A: break;
                case 0x29: break;
                case 0x33: break;
                case 0x55: break;
                case 0x65: break;
                default: break;
            }

            PC += 2;
            break;
        }

        default: {
            break;
        }
    }
}