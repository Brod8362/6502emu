#ifndef EMUSTATE_H
#define EMUSTATE_H

#include "stdint.h"

typedef uint8_t register;

typedef struct emustate {
    register a;
    register x;
    register y;
    register sr;
    register sp;
    uint16_t pc;
    char memory[256][256];
} emustate;

#endif