#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"
#include "emustate.h"

/*
immediate value
*/
typedef uint16_t imd_t; 

/*
relative, only used for branch instructions
represents offset from current PC to take
*/
typedef int8_t rel_t;

/*
memory location
*/
typedef uint16_t abs_t;

/*
zero-page, basically absolute but defaults to first page of memory 
*/
typedef uint8_t zpg_t;

/*
indirect, only used for jump instruction
set PC to address stored at this address (its odd)
*/
typedef uint16_t indr_t;

/*
number of cycles it takes for an instruction of execute
*/
typedef uint16_t cycles_t;

enum instruction_type {
    Absolute, Immediate, Implied, Indirect, Relative, Zeropage
};

union instruction_func {
    cycles_t (*absolute) (emustate*, abs_t);
    cycles_t (*immediate) (emustate*, imd_t);
    cycles_t (*implied) (emustate*);
    cycles_t (*indirect) (emustate*, indr_t);
    cycles_t (*relative) (emustate*, rel_t);
    cycles_t (*zpg) (emustate*, zpg_t);
};

typedef struct instr_info {
    const char* name;
    const uint8_t opcode;
    const enum instruction_type type;
    const union instruction_func fptr;
} instr_info;

#endif