#include <assert.h>
#include <stdio.h>

#include "emustate.h"
#include "instructions.h"

void reset_proc(emustate* emu) {
    emu->a=0;
    emu->pc=0;
    emu->sp=0;
    emu->sr=(1 << 5); //bit 5 should always be set
    emu->x=0;
    emu->y=0;
}

int main() {
    emustate emu;
    reset_proc(&emu);

    i_lda_imd(&emu, 0x05); //load 5 into the accumulator
    assert(emu.a == 0x05);
    i_sta_zpg(&emu, 0x00); //store accumulator into ZPG memory address 0
    assert(emu.memory[0][0] == 0x05); 

    i_tax(&emu); //transfer accumulator to X
    assert(emu.a == emu.x);

    i_tay(&emu); //transfer accumulator to Y
    assert(emu.a == emu.y);

    i_ldy_imd(&emu, 0x56); //load 0x56 into Y
    assert(emu.y == 0x56);

    i_tya(&emu); //transfer Y to accumulator
    assert(emu.y == emu.a);

    i_ldx_imd(&emu, 0x73); //load 0x73 into X
    assert(emu.x == 0x73);

    i_txa(&emu); //ttransfer X to accumulator
    assert(emu.x == emu.a);

    i_ldx_imd(&emu, 0); //set X to 0
    i_sta_abs_x(&emu, 0x2133); //store A into memory address $2133+X ($2133)
    assert(emu.memory[0x21][0x33] == 0x73);
    i_inx(&emu); //increment X
    i_lda_imd(&emu, 0x25); //load 0x25 into accumulator
    i_sta_abs_x(&emu, 0x2133); //store A into memory address $2133+X( $2134)
    assert(emu.memory[0x21][0x34] == 0x25);

    reset_proc(&emu);

    

    printf("All tests passed.\n");
    return 0;
}