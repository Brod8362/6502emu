#include "types.h"
#include "emustate.h"

#define ADDR(e,x) e->memory[(x)/256][(x)%256]
#define ZPG(e,x) e->memory[0][(x)%256]
#define BRANCH_CYCLES(e,offset) (e->pc/256 != (offset+e->pc)/256) ? 2 : 1

/*
emustate* emu: the emulator/processor state
indr_t opr: address to index to find new address
return: 16-bit address stored at address opr
*/
abs_t u_fetch_indr_x(emustate* emu, indr_t opr);

/*
emustate* emu: the emulator/processor state
indr_t opr: address to index to find new address
cycles_t* cycle_count: pointer to cycles_t which will include the extra # of cycles if a page boundary is crossed. (May be NULL)
return: 16-bit address stored at address opr
*/
abs_t u_fetch_indr_y(emustate* emu, indr_t opr, cycles_t* cycle_count);

/*
emustate* emu: the emulator/processor state
uint8_t reg: the value of the register that will be used to index (e.g, X or Y)
abs_t opr: memory address to access
cycles_t* cycle_count: pointer to cycles_t which will include the extra # of cycles if a page boundary is crossed. (May be NULL)
return: the value stored at 16-bit address opr+reg
*/
uint8_t u_fetch_abs_reg(emustate* emu, uint8_t reg, abs_t opr, cycles_t* cycle_count);