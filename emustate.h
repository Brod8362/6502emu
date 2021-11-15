#ifndef EMUSTATE_H
#define EMUSTATE_H

#define FLAG_N 7
#define FLAG_V 6
//flag 5 does not exist
#define FLAG_B 4
#define FLAG_D 3
#define FLAG_I 2
#define FLAG_Z 1
#define FLAG_C 0

#define SET(n,b) (n |= 1 << b)
#define CLEAR(n,b) (n &= ~(1 << b))
#define TOGGLE(n,b) (n ^= (1 << b))
#define CHECK(n,b) ((n >> b) & 1)

#include "stdint.h"

typedef struct emustate {
    //Accumulator
    uint8_t a;
    //X Register
    uint8_t x;
    //Y Register
    uint8_t y;
    // Status Register
    uint8_t sr;
    // Stack Pointer
    uint8_t sp;
    // Program Counter
    uint16_t pc;
    uint8_t memory[256*256];
} emustate;

#endif