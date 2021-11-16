#include "addr_idx.h"
#include <stdlib.h>

abs_t u_fetch_indr_x(emustate* emu, indr_t opr) {
    abs_t adr = opr+emu->x;
    uint8_t lo = ADDR(emu, adr);
    adr++;
    uint8_t hi = ADDR(emu, adr);
    return (lo | (hi << 8));
}

abs_t u_fetch_indr_y(emustate* emu, indr_t opr, cycles_t* cycle_count) {
    abs_t adr = opr;
    uint8_t lo = ADDR(emu, adr);
    adr++;
    uint8_t hi = ADDR(emu, adr);
    if (cycle_count != NULL)
        *cycle_count = ((adr/256) - (adr-1)/256);
    return (lo | (hi << 8))+emu->y;
}

uint8_t u_fetch_abs_reg(emustate* emu, uint8_t reg, abs_t opr, cycles_t* cycle_count) {
    abs_t adr = reg+opr;
    if (cycle_count != NULL && reg/256 != adr/256)
        *cycle_count = 1;
    return ADDR(emu, adr);
}