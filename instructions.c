#include "instructions.h"
#include "addr_idx.h"
#include <stdlib.h> //for NULL

/*

*: 
    add 1 to cycles if page boundary is crossed

**: 
    add 1 to cycles if branch occurs on same page
    add 2 to cycles if branch occurs to different page

*/

// ADC instruction

void g_adc(emustate* emu, uint8_t opr) {
    uint16_t s = emu->a + opr + CHECK(emu->sr, FLAG_C);

    if (s > 255) {
        SET(emu->sr, FLAG_C);
    } else {
        CLEAR(emu->sr, FLAG_C);
    }

    if (CHECK(s, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }

    if (s == 0) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }  

    if (CHECK(s, 7) != CHECK(emu->a, 7)) {
        SET(emu->sr, FLAG_V);
    } else {
        CLEAR(emu->sr, FLAG_V);
    }

    emu->a = s & 0xFF;
}

cycles_t i_adc_indr_x(emustate* emu, indr_t opr) {
    int adr = u_fetch_indr_x(emu, opr);
    g_adc(emu, ADDR(emu, adr));
    return 6;
}

cycles_t i_adc_zpg(emustate* emu, zpg_t opr) {
    g_adc(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_adc_imd(emustate* emu, imd_t opr) {
    g_adc(emu, opr);
    return 2;
}

cycles_t i_adc_abs(emustate* emu, abs_t opr) {
    g_adc(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_adc_indr_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_adc(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_adc_zpg_x(emustate* emu, zpg_t opr) {
    g_adc(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_adc_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_adc(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
}

cycles_t i_adc_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_adc(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*
}

// AND instruction

void g_and(emustate* emu, uint8_t opr) {
    CLEAR(emu->sr, FLAG_Z);
    CLEAR(emu->sr, FLAG_N);
    emu->a &= opr;

    if (emu->a == 0)
        SET(emu->sr, FLAG_Z);

    if (CHECK(emu->a, 7)) 
        SET(emu->sr, FLAG_N);
}

cycles_t i_and_indr_x(emustate* emu, indr_t opr) {
    g_and(emu, u_fetch_indr_x(emu, opr));
    return 6;
}

cycles_t i_and_zpg(emustate* emu, zpg_t opr) {
    g_and(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_and_imd(emustate* emu, imd_t opr) {
    g_and(emu, opr);
    return 2;
}

cycles_t i_and_abs(emustate* emu, abs_t opr) {
    g_and(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_and_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_and(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_and_zpg_x(emustate* emu, zpg_t opr) {
    g_and(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_and_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_and(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
}

cycles_t i_and_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_and(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*
}

// ASL instruction

void g_asl(emustate* emu, uint8_t* opr) {
    if (CHECK(*opr, 7))
        SET(emu->sr, FLAG_C);
    else
        CLEAR(emu->sr, FLAG_C);

    *opr <<= 1;

    if (*opr == 0)
        SET(emu->sr, FLAG_Z);
    else
        CLEAR(emu->sr, FLAG_Z);

    if (CHECK(*opr, 7))
        SET(emu->sr, FLAG_N);
    else
        CLEAR(emu->sr, FLAG_N);

}

cycles_t i_asl_zpg(emustate* emu, zpg_t opr) {
    g_asl(emu, &ZPG(emu, opr));   
    return 5; 
}

cycles_t i_asl_a(emustate* emu) {
    g_asl(emu, &emu->a);
    return 2;
}

cycles_t i_asl_abs(emustate* emu, abs_t opr) {
    g_asl(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_asl_zpg_x(emustate* emu, zpg_t opr) {
    g_asl(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_asl_abs_x(emustate* emu, abs_t opr) {
    g_asl(emu, &ADDR(emu, opr+emu->x));
    return 7;
}

// BCC instruction

cycles_t i_bcc_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (!CHECK(emu->sr, FLAG_C)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BCS instruction

cycles_t i_bcs_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (CHECK(emu->sr, FLAG_C)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BEQ instruction

cycles_t i_beq_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (CHECK(emu->sr, FLAG_Z)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BIT instruction

void g_bit(emustate* emu, uint8_t opr) {
    if (CHECK(opr, 7)) {
        SET(emu->sr, 7);
    } else {
        CLEAR(emu->sr, 7);
    }

    if (CHECK(opr, 6)) {
        SET(emu->sr, 6);
    } else {
        CLEAR(emu->sr, 6);
    }

    if (!(opr & emu->a)) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }
}

cycles_t i_bit_zpg(emustate* emu, zpg_t opr) {
    g_bit(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_bit_abs(emustate* emu, abs_t opr) {
    g_bit(emu, ADDR(emu, opr));
    return 4;
}

// BMI instruttion

cycles_t i_bmi_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (CHECK(emu->sr, FLAG_N)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BNE instruction

cycles_t i_bne_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (!CHECK(emu->sr, FLAG_Z)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BPL instruction

cycles_t i_bpl_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (!CHECK(emu->sr, FLAG_N)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BRK instruction

cycles_t i_brk(emustate* emu) {
    // SET(emu->sr, FLAG_I);
    // emu->memory[1][emu->sp++] = emu->pc+2;
    //TODO finish this, do more research on what it really does
    //stores PC+2 into SP?
    return 7;
}

// BVC instruction

cycles_t i_bvc_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (!CHECK(emu->sr, FLAG_V)) {
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// BVS

cycles_t i_bvs_rel(emustate* emu, rel_t opr) {
    cycles_t c = 0;
    if (CHECK(emu->sr, FLAG_V)) {        
        c = BRANCH_CYCLES(emu, opr);
        emu->pc+=opr;
    }
    return 2 + c; //**
}

// CLC instruction

cycles_t i_clc(emustate* emu) {
    CLEAR(emu->sr, FLAG_C);
    return 2;
}

// CLD instruction

cycles_t i_cld(emustate* emu) {
    CLEAR(emu->sr, FLAG_D);
    return 2;
}

// CLI instruction

cycles_t i_cli(emustate* emu) {
    CLEAR(emu->sr, FLAG_I);
    return 2;
}

// CLV instruction

cycles_t i_clv(emustate* emu) {
    CLEAR(emu->sr, FLAG_V);
    return 2;
}

// CMP instruction

void g_comp_generic(emustate* emu, uint8_t reg, uint8_t opr) {
    uint8_t res = reg - opr;
    if (!res)
        SET(emu->sr, FLAG_Z);
    else
        CLEAR(emu->sr, FLAG_Z);
    
    if (CHECK(res, 7))
        SET(emu->sr, FLAG_N);
    else 
        CLEAR(emu->sr, FLAG_N); 

    if (opr <= reg)
        SET(emu->sr, FLAG_C);
    else
        CLEAR(emu->sr, FLAG_C);
}

void g_cmp(emustate* emu, uint8_t opr) {
    g_comp_generic(emu, emu->a, opr);
}

cycles_t i_cmp_zpg_x(emustate* emu, zpg_t opr) {
    g_cmp(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_cmp_indr_x(emustate* emu, indr_t opr) {
    int adr = u_fetch_indr_x(emu, opr);
    g_cmp(emu, ADDR(emu, adr));
    return 6;
}

cycles_t i_cmp_zpg(emustate* emu, zpg_t opr) {
    g_cmp(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_cmp_imd(emustate* emu, imd_t opr) {
    g_cmp(emu, opr);
    return 2;
}

cycles_t i_cmp_abs(emustate* emu, abs_t opr) {
    g_cmp(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_cmp_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_cmp(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_cmp_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_cmp(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
}

cycles_t i_cmp_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_cmp(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*
}

// CPX instruction

void g_cpx(emustate* emu, uint8_t opr) {
    g_comp_generic(emu, emu->x, opr);
}

cycles_t i_cpx_imd(emustate* emu, imd_t opr) {
    g_cpx(emu, opr);
    return 2;
}

cycles_t i_cpx_zpg(emustate* emu, zpg_t opr) {
    g_cpx(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_cpx_abs(emustate* emu, abs_t opr) {
    g_cpx(emu, ADDR(emu, opr));
    return 4;
}

// CPY instruction

void g_cpy(emustate* emu, uint8_t opr) {
    g_comp_generic(emu, emu->y, opr);
}

cycles_t i_cpy_imd(emustate* emu, imd_t opr) {
    g_cpy(emu, opr);
    return 2;
}

cycles_t i_cpy_zpg(emustate* emu, zpg_t opr) {
    g_cpy(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_cpy_abs(emustate* emu, abs_t opr) {
    g_cpy(emu, ADDR(emu, opr));
    return 4;
}

// DEC instruction

void g_decr(emustate* emu, uint8_t* reg) {
    (*reg)--;

    if (*reg == 0) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }

    if (CHECK(*reg, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }
}

cycles_t i_dec_zpg(emustate* emu, zpg_t opr) {
    g_decr(emu, &ZPG(emu, opr));
    return 5;
}

cycles_t i_dec_abs(emustate* emu, abs_t opr) {
    g_decr(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_dec_zpg_x(emustate* emu, zpg_t opr) {
    g_decr(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_dec_abs_x(emustate* emu, abs_t opr) {
    g_decr(emu, &ADDR(emu, opr+emu->x));
    return 7;
}

// DEX instruction

cycles_t i_dex(emustate* emu) {
    g_decr(emu, &emu->x);
    return 2;
}

// DEY instruction

cycles_t i_dey(emustate* emu) {
    g_decr(emu, &emu->y);
    return 2;
}

// EOR instruction

void g_eor(emustate* emu, uint8_t opr) {
    emu->a ^= opr;

    if (emu->a == 0) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }

    if (CHECK(emu->a, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }
}

cycles_t i_eor_indr_x(emustate* emu, indr_t opr) {
    int adr = u_fetch_indr_x(emu, opr);
    g_eor(emu, ADDR(emu, adr));
    return 6;
}

cycles_t i_eor_zpg(emustate* emu, zpg_t opr) {
    g_eor(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_eor_imd(emustate* emu, imd_t opr) {
    g_eor(emu, opr);
    return 2;
}

cycles_t i_eor_abs(emustate* emu, abs_t opr) {
    g_eor(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_eor_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_eor(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_eor_zpg_x(emustate* emu, zpg_t opr) {
    g_eor(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_eor_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_eor(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
}

cycles_t i_eor_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_eor(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*  
}

// INC instruction

void g_incr(emustate* emu, uint8_t* reg) {
    (*reg)++;

    if (*reg == 0) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }

    if (CHECK(*reg, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }
}

cycles_t i_inc_zpg(emustate* emu, zpg_t opr) {
    g_incr(emu, &ZPG(emu, opr));
    return 5;
}

cycles_t i_inc_abs(emustate* emu, abs_t opr) {
    g_incr(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_inc_zpg_x(emustate* emu, zpg_t opr) {
    g_incr(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_inc_abs_x(emustate* emu, abs_t opr) {
    g_incr(emu, &ADDR(emu, opr+emu->x));
    return 7;
}

// INX instruction

cycles_t i_inx(emustate* emu) {
    g_incr(emu, &emu->x);
    return 2;
}

// INY instruction

cycles_t i_iny(emustate* emu) {
    g_incr(emu, &emu->y);
    return 2;
}

// JMP instruction

cycles_t i_jmp_abs(emustate* emu, abs_t opr) {
    emu->pc = opr;
    return 3;
}

cycles_t i_jmp_indr(emustate* emu, indr_t opr) {
    emu->pc = ADDR(emu, opr);
    return 5;
}

// JSR instruction

cycles_t i_jsr_abs(emustate* emu, abs_t opr) {
    emu->memory[1][emu->sp--] = emu->pc/256;
    emu->memory[1][emu->sp--] = emu->pc%256;
    emu->pc = opr;
    return 6;
}

// LDA instruction

cycles_t i_lda_indr_x(emustate* emu, indr_t opr) {
    abs_t adr = u_fetch_indr_x(emu, opr);
    emu->a = ADDR(emu, adr);
    return 6;
}

cycles_t i_lda_imd(emustate* emu, imd_t opr) {
    emu->a = opr;
    return 2;
}

cycles_t i_lda_abs(emustate* emu, abs_t opr) {
    emu->a = ADDR(emu, opr);
    return 4;
}

cycles_t i_lda_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    emu->a = ADDR(emu, adr);
    return 5 + xtra; //*
}

cycles_t i_lda_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    emu->a = u_fetch_abs_reg(emu, emu->y, opr, &xtra);
    return 4 + xtra; //*
}

cycles_t i_lda_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    emu->a = u_fetch_abs_reg(emu, emu->x, opr, &xtra);
    return 4 + xtra; //*
}

cycles_t i_lda_zpg_x(emustate* emu, zpg_t opr) {
    emu->a = ZPG(emu, opr+emu->x);
    return 4;
}

// LDX instruction

cycles_t i_ldx_imd(emustate* emu, imd_t opr) {
    emu->x = opr;
    return 2;
}

cycles_t i_ldx_zpg(emustate* emu, zpg_t opr) {
    emu->x = ZPG(emu, opr);
    return 3;
}

cycles_t i_ldx_abs(emustate* emu, abs_t opr) {
    emu->x = ADDR(emu, opr);
    return 4;
}

cycles_t i_ldx_zpg_y(emustate* emu, zpg_t opr) {
    emu->x = ZPG(emu, opr+emu->y);
    return 4;
}

cycles_t i_ldx_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    emu-> x = u_fetch_abs_reg(emu, emu->y, opr, &xtra);
    return 4 + xtra; //*
}

// LDY instruction

cycles_t i_ldy_imd(emustate* emu, imd_t opr) {
    emu->y = opr;
    return 2;
}

cycles_t i_ldy_zpg(emustate* emu, zpg_t opr) {
    emu->y = ZPG(emu, opr);
    return 3;
}

cycles_t i_ldy_zpg_x(emustate* emu, zpg_t opr) {
    emu->y = ZPG(emu, opr+emu->x);
    return 4;
}

cycles_t i_ldy_abs(emustate* emu, abs_t opr) {
    emu->y = ADDR(emu, opr);
    return 4;
}

cycles_t i_ldy_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    emu->y = u_fetch_abs_reg(emu, emu->x, opr, &xtra);
    return 4 + xtra; //*
}

// LSR instruction

void g_lsr(emustate* emu, uint8_t* opr) {
    CLEAR(emu->sr, FLAG_N); //always reset

    if (CHECK(*opr, 0))
        SET(emu->sr, FLAG_C);
    else
        CLEAR(emu->sr, FLAG_C);

    *opr >>= 1;

    if (*opr == 0)
        SET(emu->sr, FLAG_Z);
    else
        CLEAR(emu->sr, FLAG_Z);
}


cycles_t i_lsr_zpg(emustate* emu, zpg_t opr) {
    g_lsr(emu, &ZPG(emu, opr));
    return 5;
}

cycles_t i_lsr_a(emustate* emu) {
    g_lsr(emu, &emu->a);
    return 2;
}

cycles_t i_lsr_abs(emustate* emu, abs_t opr) {
    g_lsr(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_lsr_zpg_x(emustate* emu, zpg_t opr) {
    g_lsr(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_lsr_abs_x(emustate* emu, abs_t opr) {
    g_lsr(emu, &ADDR(emu, opr+emu->x));
    return 7;
}

// NOP instruction

cycles_t i_nop(emustate* emu) {
    return 2;
}

// ORA instruction

void g_ora(emustate* emu, uint8_t opr) {
    CLEAR(emu->sr, FLAG_Z);
    CLEAR(emu->sr, FLAG_N);

    emu->a |= opr;
    
    if (emu->a == 0)
        SET(emu->sr, FLAG_Z);

    if (CHECK(emu->a, 7))
        SET(emu->sr, FLAG_N);
}

cycles_t i_ora_indr_x(emustate* emu, indr_t opr) {
    abs_t adr = u_fetch_indr_x(emu, opr);
    g_ora(emu, ADDR(emu, adr));
    return 6;
}

cycles_t i_ora_zpg(emustate* emu, zpg_t opr) {
    g_ora(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_ora_imd(emustate* emu, imd_t opr) {
    g_ora(emu, opr);
    return 2;
}

cycles_t i_ora_abs(emustate* emu, abs_t opr) {
    g_ora(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_ora_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_ora(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_ora_zpg_x(emustate* emu, zpg_t opr) {
    g_ora(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_ora_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_ora(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
}

cycles_t i_ora_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_ora(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*
}

// PHA instruction

cycles_t i_pha(emustate* emu) {
    emu->memory[1][emu->sp--] = emu->a;
    return 3;
}

// PHP insturction

cycles_t i_php(emustate* emu) {
    emu->memory[1][emu->sp--] = emu->sr;
    return 3;
}

// PLA instruction

cycles_t i_pla(emustate* emu) {
    emu->a = emu->memory[1][++emu->sp];
    return 4;
}

// PLP instruction

cycles_t i_plp(emustate* emu) {
    emu->sr = emu->memory[1][++emu->sp];
    return 4;
}

// ROL instruction

void g_rol(emustate* emu, uint8_t* opr) {
    uint8_t init_carry = CHECK(*opr, 7); //save 7th bit for later
    *opr <<= 1; //shift left
    *opr |= CHECK(emu->sr, FLAG_C); //add carry in
    if (init_carry) //set carry to 7th bit 
        SET(emu->sr, FLAG_C);
    else
        CLEAR(emu->sr, FLAG_C);

    if (CHECK(emu->sr, 7)) //initial 6th bit
        SET(emu->sr, FLAG_N);
    else
        CLEAR(emu->sr, FLAG_N);

    if (*opr == 0)
        SET(emu->sr, FLAG_Z);
    else
        CLEAR(emu->sr, FLAG_Z);
    
}

cycles_t i_rol_zpg(emustate* emu, zpg_t opr) {
    g_rol(emu, &ZPG(emu, opr));
    return 5;
}

cycles_t i_rol_a(emustate* emu) {
    g_rol(emu, &emu->a);
    return 2;
}

cycles_t i_rol_abs(emustate* emu, abs_t opr) {
    g_rol(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_rol_zpg_x(emustate* emu, zpg_t opr) {
    g_rol(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_rol_abs_x(emustate* emu, abs_t opr) {
    g_rol(emu, &ADDR(emu, opr+emu->x));
    return 7;
}

// ROR instruction

void g_ror(emustate* emu, uint8_t* opr) {
    uint8_t bit_0 = *opr & 1;
    *opr >>= 1; //shift right
    *opr |= (CHECK(emu->sr, FLAG_C) << 7); //add carry in

    if (CHECK(emu->sr, FLAG_C)) //initial 6th bit
        SET(emu->sr, FLAG_N);
    else
        CLEAR(emu->sr, FLAG_N);

    if (bit_0)
        SET(emu->sr, FLAG_C);
    else
        CLEAR(emu->sr, FLAG_C);

    if (*opr == 0)
        SET(emu->sr, FLAG_Z);
    else
        CLEAR(emu->sr, FLAG_Z);
}

cycles_t i_ror_zpg(emustate* emu, zpg_t opr) {
    g_ror(emu, &ZPG(emu, opr));
    return 5;
}

cycles_t i_ror_a(emustate* emu) {
    g_ror(emu, &emu->a);
    return 2;
}

cycles_t i_ror_abs(emustate* emu, abs_t opr) {
    g_ror(emu, &ADDR(emu, opr));
    return 6;
}

cycles_t i_ror_zpg_x(emustate* emu, zpg_t opr) {
    g_ror(emu, &ZPG(emu, opr+emu->x));
    return 6;
}

cycles_t i_ror_abs_x(emustate* emu, abs_t opr) {
    g_ror(emu, &ADDR(emu, emu->x+opr));
    return 7;
}

// RTI instruction
// RTS instruction

cycles_t i_rts(emustate* emu) {
    uint8_t low = emu->memory[1][++emu->sp];
    uint8_t high = emu->memory[1][++emu->sp];
    emu->pc = low | (high << 8);
    return 6;
}

// SBC instruction

void g_sbc(emustate* emu, uint8_t opr) {
    int16_t res = emu->a + ~opr + CHECK(emu->sr, FLAG_C); //TODO check this is correct
    if (res >= 0) {
        SET(emu->sr, FLAG_C);
    } else {
        CLEAR(emu->sr, FLAG_C);
    }

    if (res > 127 || res < -127) {
        SET(emu->sr, FLAG_V);
    } else {
        CLEAR(emu->sr, FLAG_V);
    }

    if (CHECK(res, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }

    if (res == 0) {
        SET(emu->sr, FLAG_Z);
    }  else {
        CLEAR(emu->sr, FLAG_Z);
    }

    emu->a = res & 0xFF;
}

cycles_t i_sbc_indr_x(emustate* emu, indr_t opr) {
    abs_t adr = u_fetch_indr_x(emu, opr);
    g_sbc(emu, ADDR(emu, adr));
    return 6;
}

cycles_t i_sbc_zpg(emustate* emu, zpg_t opr) {
    g_sbc(emu, ZPG(emu, opr));
    return 3;
}

cycles_t i_sbc_imd(emustate* emu, imd_t opr) {
    g_sbc(emu, opr);
    return 2;
}

cycles_t i_sbc_abs(emustate* emu, abs_t opr) {
    g_sbc(emu, ADDR(emu, opr));
    return 4;
}

cycles_t i_sbc_indr_y(emustate* emu, indr_t opr) {
    cycles_t xtra = 0;
    abs_t adr = u_fetch_indr_y(emu, opr, &xtra);
    g_sbc(emu, ADDR(emu, adr));
    return 5 + xtra; //*
}

cycles_t i_sbc_zpg_x(emustate* emu, zpg_t opr) {
    g_sbc(emu, ZPG(emu, opr+emu->x));
    return 4;
}

cycles_t i_sbc_abs_y(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_sbc(emu, u_fetch_abs_reg(emu, emu->y, opr, &xtra));
    return 4 + xtra; //*
} 

cycles_t i_sbc_abs_x(emustate* emu, abs_t opr) {
    cycles_t xtra = 0;
    g_sbc(emu, u_fetch_abs_reg(emu, emu->x, opr, &xtra));
    return 4 + xtra; //*
}

// SEC instruction

cycles_t i_sec(emustate* emu) {
    SET(emu->sr, FLAG_C);
    return 2;
}

// SED instruction

cycles_t i_sed(emustate* emu) {
    SET(emu->sr, FLAG_D);
    return 2;
}

// SEI instruction

cycles_t i_sei(emustate* emu) {
    SET(emu->sr, FLAG_I);
    return 2;
}

// STA instruction

cycles_t i_sta_indr_x(emustate* emu, indr_t opr) {
    int adr = u_fetch_indr_x(emu, opr);
    ADDR(emu, adr) = emu->a;
    return 6;
}

cycles_t i_sta_zpg(emustate* emu, zpg_t opr) {
    ZPG(emu, opr) = emu->a;
    return 3;
}

cycles_t i_sta_abs(emustate* emu, abs_t opr) {
    ADDR(emu,opr) = emu->a;
    return 4;
}

cycles_t i_sta_indr_y(emustate* emu, indr_t opr) {
    abs_t adr = u_fetch_indr_y(emu, opr, NULL);
    ADDR(emu, adr) = emu->a;
    return 6;
}

cycles_t i_sta_zpg_x(emustate* emu, zpg_t opr) {
    ZPG(emu, opr+emu->x) = emu->a;
    return 4;
}

cycles_t i_sta_abs_y(emustate* emu, abs_t opr) {
    ADDR(emu, opr+emu->y) = emu->a;
    return 5;
}

cycles_t i_sta_abs_x(emustate* emu, abs_t opr) {
    ADDR(emu, opr+emu->x) = emu->a;
    return 5;
}

// STX instruction

cycles_t i_stx_zpg(emustate* emu, zpg_t opr) {
    ZPG(emu, opr) = emu->x;
    return 3;
}

cycles_t i_stx_abs(emustate* emu, abs_t opr) {
    ADDR(emu, opr) = emu->x;
    return 4;
}

cycles_t i_stx_zpg_y(emustate* emu, zpg_t opr) {
    ZPG(emu, opr+emu->y) = emu->x;
    return 4;
}

// STY instruction

cycles_t i_sty_zpg(emustate* emu, zpg_t opr) {
    ZPG(emu, opr) = emu->y;
    return 3;
}

cycles_t i_sty_abs(emustate* emu, abs_t opr) {
    ADDR(emu, opr) = emu->y;
    return 4;
}

cycles_t i_sty_zpg_x(emustate* emu, zpg_t opr) {
    ZPG(emu, opr+emu->x) = emu->y;
    return 4;
}

// TAX instruction

void g_txx_generic(emustate* emu, const uint8_t* source, uint8_t* dest) {
    *dest = *source;

    if (CHECK(*dest, 7)) {
        SET(emu->sr, FLAG_N);
    } else {
        CLEAR(emu->sr, FLAG_N);
    }

    if (*dest == 0) {
        SET(emu->sr, FLAG_Z);
    } else {
        CLEAR(emu->sr, FLAG_Z);
    }
}

cycles_t i_tax(emustate* emu) {
    g_txx_generic(emu, &emu->a, &emu->x);
    return 2;
}

// TAY instruction

cycles_t i_tay(emustate* emu) {
    g_txx_generic(emu, &emu->a, &emu->y);
    return 2;
}

// TSX instruction

cycles_t i_tsx(emustate* emu) {
    g_txx_generic(emu, &emu->sp, &emu->x);
    return 2;
}

// TXA instruction

cycles_t i_txa(emustate* emu) {
    g_txx_generic(emu, &emu->x, &emu->a);
    return 2;
}

// TXS instruction

cycles_t i_txs(emustate* emu) {
    //IMPORTANT: this instruction does not set the carry or zero flags
    emu->sp = emu->x;
    return 2;
}

// TYA instruction

cycles_t i_tya(emustate* emu) {
    g_txx_generic(emu, &emu->y, &emu->a);
    return 2;
}