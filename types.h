#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"


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

#endif