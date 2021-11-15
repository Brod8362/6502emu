#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"


/*
immediate value
*/
typedef uint16_t imd_t; 


// NON-INDEXED TYPES 

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

// INDEXED TYPES

/* 
indexed absolute X
basically, target address will be determined by adding X register to this value
*/
typedef uint16_t abs_x_t;

/* 
indexed absolute Y
basically, target address will be determined by adding Y register to this value
*/
typedef uint16_t abs_y_t;

/*
zero page X-indexed
limited to $FF
*/
typedef uint8_t zpg_x_t;

/*
zero page Y-indexed
limited to $FF
*/
typedef uint8_t zpg_y_t;

/*
indexed indirect, only used with X register
add X register to this value, then use that address to fetch the value stored there.
*/
typedef uint16_t indr_x_t;

/*
indirect indexed
fetch the value from this address, then add Y to it 
*/
typedef uint16_t indr_y_t;


#endif