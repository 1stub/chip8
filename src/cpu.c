#include "../include/cpu.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>

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

/* Nasty but no mallocs! Fuck the heap! */
byte V[16];
word stack[16];
word PC;
word SP;
word I;
byte delay_timer;
byte sound_timer;
bool keyboard_status[CHIP8_KEY_COUNT];
bool legacy;
bool can_render;

void cpu_init() {
    int i;
    PC = CHIP8_ROM_BP;
    SP = 0x0000;
    I  = 0x0000;
    delay_timer = 0;
    sound_timer = 0;
    legacy = true;
    can_render = true;

    for(i = 0; i < 80; i++) {
        memory[i] = chip8_font[i];
    }

    memset(V, 0, sizeof(byte) * 16);
    memset(stack, 0, sizeof(word) * 16);
    memset(keyboard_status, false, sizeof(bool) * CHIP8_KEY_COUNT);
}

/** 
* For instructions that I wrote are ambiguous, this means their functionality
* differ depending on the interpreter used. For now I have picked the most
* common functionality, but this may change if I decide to really make this
* emulator work well.
**/
void execute(int tick) {
    word opcode = read(PC) << 8 | read(PC + 1);
    word opcode_first_nibble = opcode >> 12; 

    debug_print("[DEBUG] Opcode 0x%4x\n", opcode);

    switch(opcode_first_nibble) {
        case 0x0: {
            if(opcode == 0x00E0) {
                memset(pixel_buffer, 0, sizeof(pixel_buffer));
                can_render = true;
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
                case   0: { 
                    V[X] = V[Y]; 
                    break;
                }
                case   1: {
                    if(legacy) {
                        V[X] = V[X] | V[Y];
                        V[0xF] = 0x00;
                    } 

                    if(!legacy) {
                        word result = V[X] | V[Y];
                        V[0xF] = (result >> 8) & 0x01;
                        V[X] |= V[Y]; 
                    }
                    break;
                }
                case   2: {
                    if(legacy) {
                        V[X] = V[X] & V[Y]; 
                        V[0xF] = 0x00;
                    }

                    if(!legacy) {
                        word result = V[X] & V[Y];
                        V[0xF] = (result >> 8) & 0x01;
                        V[X] &= V[Y]; 
                    }
                    break;
                }
                case   3: {
                    if(legacy) {
                        V[X] = V[X] ^ V[Y]; 
                        V[0xF] = 0x00;
                    }

                    if(!legacy) {
                        word result = V[X] ^ V[Y];
                        V[0xF] = (result >> 8) & 0x01;
                        V[X] = result; 
                    }
                    break;
                } 
                case   4: {
                    word sum = V[X] + V[Y];
                    V[X] = (byte)sum;
                    V[0xF] = sum > 255 ? true : false;
                } 
                break;
                case   5: {
                    bool should_flag = (V[X] >= V[Y]) ? true : false;
                    V[X] = V[X] - V[Y];
                    V[0xF] = (byte)should_flag;
                }
                break;
                case   6: {
                    /* Some ambiguities to this instr */
                    word flag = V[X] & 0x0001;
                    if(legacy) { V[X] = V[Y]; }
                    V[X] >>= 1;
                    V[0xF] = flag;
                } break;
                case   7: {
                    V[X] = V[Y] - V[X]; 
                    V[0xF] = (V[Y] > V[X]) ? true : false;
                }break;
                case 0xE: { 
                    /* Some ambiguities to this instr */
                    bool flag = V[X] & 0x80;
                    if(legacy) { V[X] = V[Y]; }
                    V[X] = V[X] << 1;
                    V[0xF] = flag ? true : false;
                }
                break; 
                default: break;
            }
            PC += 2;
            break;
        }
        case 0x9: {
            if(V[X] != V[Y]) {
                PC += 4;
            } else {
                PC += 2;
            }
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
            int x_pos = V[X] & (CHIP8_WIDTH - 1);
            int y_pos = V[Y] & (CHIP8_HEIGHT - 1);
            int idx, pixel;

            V[0xF] = 0;
            for (idx = 0; idx < N; idx++) {
                byte data = read(I + idx);
                if(y_pos + idx >= CHIP8_HEIGHT) break;

                for (pixel = 0; pixel < 8; pixel++) {
                    byte sprite_pixel = GET_BIT(data, pixel);
                    int x_coord = x_pos + (7 - pixel);
                    bool* display_pixel = &pixel_buffer[(y_pos + idx)][x_coord];
                    if(x_coord >= CHIP8_WIDTH) continue;


                    if(sprite_pixel != 0 && *display_pixel != 0){
                            V[0xF] = 1;
                    }
                    *display_pixel ^= sprite_pixel; /* XOR draw */
                }
            }
            can_render = true;
            PC += 2;
            break;
        }
        case 0xE: {
            debug_print("Keycode in V[X]: %x, Is keypressed?: %x\n", V[X], keyboard_status[V[X]]);
            switch(NN) {
                case 0x9E: { 
                    if (keyboard_status[V[X]]) {
                        PC += 4; 
                    } else {
                        PC += 2;
                    }
                }
                break;

                case 0xA1: { 
                    if (!keyboard_status[V[X]]) {
                        PC += 4;
                    } else {
                        PC += 2;
                    }
                }
                break;

                default: break;
            }
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

                case 0x0A: {
                    int i;
                    static bool any_key_pressed = false;
                    static int key = -1;

                    debug_print("[DEBUG] Wait for key instruction\n");
                    for(i = 0; i < CHIP8_KEY_COUNT; i++) {
                        if(keyboard_status[i]) {
                            PC += 2;
                            any_key_pressed = true;
                            key = i;
                        }
                    }
                    
                    if(!any_key_pressed) PC -= 2;
                    else {
                        if(keyboard_status[key] == 0) {
                            V[X] = key;
                            key = -1;
                            any_key_pressed = false;
                        }else {
                            PC -= 2;
                        }
                    }

                    break;
                }

                case 0x29: {
                    debug_print("I = location of font for character V[0x%x] = 0x%x\n", X, V[X]);
                    I = V[X] * 5;
                }
                break;

                case 0x33: {
                    byte vx_val = V[X];
                    byte digits[3];
                    int idx;

                    /* Extract digits in reverse order */
                    for(idx = 2; idx >= 0; idx--) {
                        digits[idx] = vx_val % 10;
                        vx_val /= 10;
                    }

                    write(I, digits[0]);
                    write(I + 1, digits[1]);
                    write(I + 2, digits[2]);
                }
                break;

                case 0x55: {
                    /* Some ambiguities to this instr */
                    int idx;
                    for(idx = 0; idx <= (X); idx++) {
                        write(I + idx, V[idx]);
                    }

                    if (legacy) {
                        I += ((X) + 1);
                    }
                }
                break;

                case 0x65: {
                    /* Some ambiguities to this instr */
                    /** 
                    * Older Chip8 interpreters modify I here,
                    * Newer interpreters (Chip48 and superchip) don't 
                    **/
                    int idx;
                    for(idx = 0; idx <= (X); idx++) {
                        V[idx] = read(I + idx);
                    }
 
                    if (legacy) {
                        I += ((X) + 1);
                    }
                }
                break;

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

void update_timers() {
    /** 
    * This doesnt do anythign when it hits zero. Not sure totally how to handle
    * timers on this system totally
    **/
    if(delay_timer > 0) {
        delay_timer--;
    }

    if(sound_timer > 0) {
        sound_timer--;
        printf("BEEP!!!!\n");
    }
}
