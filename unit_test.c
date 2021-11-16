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
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            emu->memory[i][j] = 0;
        }
    }
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

    //TEST CLEAR/SET
    assert(CHECK(emu.sr, FLAG_C) == 0);
    assert(CHECK(emu.sr, FLAG_D) == 0);
    assert(CHECK(emu.sr, FLAG_V) == 0);
    assert(CHECK(emu.sr, FLAG_I) == 0);

    i_sec(&emu);
    i_sed(&emu);
    i_sei(&emu);
    //TODO perform an operation that sets overflow bit
    SET(emu.sr, FLAG_V);

    assert(CHECK(emu.sr, FLAG_C) == 1);
    assert(CHECK(emu.sr, FLAG_D) == 1);
    assert(CHECK(emu.sr, FLAG_V) == 1);
    assert(CHECK(emu.sr, FLAG_I) == 1);

    i_clc(&emu);
    i_cld(&emu);
    i_clv(&emu);
    i_cli(&emu);

    assert(CHECK(emu.sr, FLAG_C) == 0);
    assert(CHECK(emu.sr, FLAG_D) == 0);
    assert(CHECK(emu.sr, FLAG_V) == 0);
    assert(CHECK(emu.sr, FLAG_I) == 0);
    
    // test ADC 

    reset_proc(&emu);

    // 13 + 211 + CARRY = (A:255, C:0)
    i_sec(&emu);
    i_lda_imd(&emu, 13);
    i_adc_imd(&emu, 211);
    assert(emu.a == 225);
    assert(CHECK(emu.sr, FLAG_C) == 0);

    // 254 + 6 + CARRY = (A:5, C:1)
    i_sec(&emu);
    i_lda_imd(&emu, 254);
    i_adc_imd(&emu, 6);
    assert(emu.a == 5);
    assert(CHECK(emu.sr, FLAG_C) == 1);

    // 5 + 102 + CARRY = (A:108, C:0)
    i_sec(&emu); //ensure carry bit is set
    i_ldx_imd(&emu, 0x66); //load 0x66 into X
    i_stx_abs(&emu, 0x2040); //store X at mem address $2040
    i_adc_abs(&emu, 0x2040); //add accumulator to value at address $2040
    assert(emu.a == 108);
    assert(CHECK(emu.sr, FLAG_C) == 0);

    //5 + 7 = (A:12, C: 0, V:0)
    i_clc(&emu); // clear carry flag
    i_lda_imd(&emu, 5); //load 5 into accumulator
    i_adc_imd(&emu, 7); // 5 + 7
    assert((int8_t)emu.a == 12);
    assert(!CHECK(emu.sr, FLAG_C));
    assert(!CHECK(emu.sr, FLAG_V)); //assert that C and V are not set

    //127 + 2 = (A:-127, C:0, V:1)
    i_clc(&emu); // clear carry flag
    i_lda_imd(&emu, 127);
    i_adc_imd(&emu, 2);
    assert((int8_t)emu.a == -127);
    assert(!CHECK(emu.sr, FLAG_C)); //C must not be set
    assert(CHECK(emu.sr, FLAG_V));  //V must be set

    //5 + -3 = (A:2, C:1, V:0)
    i_clc(&emu); // clear carry flag
    i_lda_imd(&emu, 5);
    i_adc_imd(&emu, ~3+1); // 5 - 3 = 2
    assert((int8_t)emu.a == 2);
    assert(CHECK(emu.sr, FLAG_C)); //C must not be set
    assert(!CHECK(emu.sr, FLAG_V));  //V must not be set

    //5 + -7 = (A:-2, C:0, V:0)
    i_clc(&emu);
    i_lda_imd(&emu, 5);
    i_adc_imd(&emu, ~7+1); // 5 + -7 = -2
    assert((int8_t)emu.a == -2); 
    assert(!CHECK(emu.sr, FLAG_C)); //C must not be set
    assert(CHECK(emu.sr, FLAG_V));  //V must be set

    //-5 + -7 = (A: -12, C:1, V:0)
    i_clc(&emu);
    i_lda_imd(&emu, ~5+1);
    i_adc_imd(&emu, ~7+1);
    assert((int8_t)emu.a == -12);
    assert(CHECK(emu.sr, FLAG_C)); //C must not be set
    assert(!CHECK(emu.sr, FLAG_V));  //V not must be set

    //-66 + -55 = (A:125, C:1, V:1)
    i_clc(&emu);
    i_lda_imd(&emu, ~66+1);
    i_adc_imd(&emu, ~65+1);
    assert((int8_t)emu.a == 125);
    assert(CHECK(emu.sr, FLAG_C)); //C must be set
    assert(CHECK(emu.sr, FLAG_V));  //V must be set

    //test AND instruction
    reset_proc(&emu);
    i_lda_imd(&emu, 0b11001111);
    i_and_imd(&emu, 0b11110111);
    assert(emu.a == 0b11000111);

    i_lda_imd(&emu, 0b00000000);
    i_and_imd(&emu, 0b11111111);
    assert(emu.a == 0b00000000);
    assert(CHECK(emu.sr, FLAG_Z));
    assert(!CHECK(emu.sr, FLAG_N));

    i_lda_imd(&emu, 0b10101100);
    i_and_imd(&emu, 0b11011100);
    assert(emu.a == 0b10001100);
    assert(!CHECK(emu.sr, FLAG_Z));
    assert(CHECK(emu.sr, FLAG_N));

    //test OR instruction
    reset_proc(&emu);
    i_lda_imd(&emu, 0b00000000);
    i_ora_imd(&emu, 0b11111111);
    assert(emu.a == 0b11111111);
    assert(!CHECK(emu.sr, FLAG_Z));
    assert(CHECK(emu.sr, FLAG_N));

    i_lda_imd(&emu, 0b10101010);
    i_ora_imd(&emu, 0b01010101);
    assert(emu.a == 0b11111111);
    assert(!CHECK(emu.sr, FLAG_Z));
    assert(CHECK(emu.sr, FLAG_N));

    i_lda_imd(&emu, 0b00000000);
    i_ora_imd(&emu, 0b00000000);
    assert(emu.a == 0b00000000);
    assert(CHECK(emu.sr, FLAG_Z));
    assert(!CHECK(emu.sr, FLAG_N));

    // test INC, INX, INY, DEC, DEX, DEY
    reset_proc(&emu);

    i_lda_imd(&emu, 0x35);
    i_sta_abs(&emu, 0x8752);
    assert(emu.memory[0x87][0x52] == 0x35);
    i_inc_abs(&emu, 0x8752);
    assert(emu.memory[0x87][0x52] == 0x36);
    i_dec_abs(&emu, 0x8752);
    assert(emu.memory[0x87][0x52] == 0x35);

    i_ldx_imd(&emu, 0x20);
    assert(emu.x == 0x20);
    i_inx(&emu);
    assert(emu.x == 0x21);
    i_dex(&emu);
    assert(emu.x == 0x20);

    i_ldy_imd(&emu, 0x20);
    assert(emu.y == 0x20);
    i_iny(&emu);
    assert(emu.y == 0x21);
    i_dey(&emu);
    assert(emu.y == 0x20);

    // test NOP
    i_nop(&emu);

    //test EOR (XOR)
    i_lda_imd(&emu, 0b11001100);
    i_eor_imd(&emu, 0b00110011);
    assert(emu.a == 0b11111111);
    assert(!CHECK(emu.sr, FLAG_Z));
    assert(CHECK(emu.sr, FLAG_N));

    i_lda_imd(&emu, 0b11111111);
    i_eor_imd(&emu, 0b11111111);
    assert(emu.a == 0b00000000);
    assert(CHECK(emu.sr, FLAG_Z));
    assert(!CHECK(emu.sr, FLAG_N));

    printf("All tests passed.\n");
    return 0;
}