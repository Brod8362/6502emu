#include "instructions.h"

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

    emu->a = (uint8_t)s;
}

// AND instruction

void g_and(emustate* emu, uint8_t opr) {
    emu->a &= opr;

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

// ASL instruction

void g_asl(emustate* emu, uint8_t* opr) {
    if (CHECK(*opr, 7))
        SET(emu->sr, FLAG_C);
    *opr <<= 1;
    //todo verify this is acutally what it does
}

void i_asl_zpg(emustate* emu, zpg_t opr) {
    g_asl(emu, &emu->memory[opr]);    
}

void i_asl_a(emustate* emu) {
    g_asl(emu, &emu->a);
}

void i_asl_abs(emustate* emu, abs_t opr) {

}

// BCC instruction
// BCS instruction
// BEQ instruction
// BIT instruction
// BMI instruttion
// BNE instruction
// BPL instruction

void i_bpl_rel(emustate* emu, rel_t opr) {

}

// BRK instruction

void i_brk(emustate* emu) {
    SET(emu->sr, FLAG_I);
    emu->memory[0x100 + emu->sp++] = emu->pc+2;
    //TODO finish this, do more research on what it really does
    //stores PC+2 into SP?
}

// BVC instruction
// CLC instruction
// CLD instruction
// CLI instruction
// CLV instruction
// CMP instruction
// CPX instruction
// CPY instruction
// DEC instruction
// DEX instruction
// DEY instruction
// EOR instruction
// INC instruction
// INX instruction
// INY instruction
// JMP instruction
// LDA instruction
// LDX instruction
// LSR instruction
// NOP instruction
// ORA instruction

/*
emu : emustate
opr : value accumulator will be or'd with
*/
void g_ora(emustate* emu, uint8_t opr) {
    emu->a |= opr;
    //TODO figure out when the flags should be set
}

void i_ora_indr_x(emustate* emu, indr_t opr) {
    g_ora(emu, emu->memory[opr+emu->x]);
}

void i_ora_zpg(emustate* emu, zpg_t opr) {
    g_ora(emu, emu->memory[opr]);
}

void i_ora_imd(emustate* emu, imd_t opr) {
    g_ora(emu, opr);
}

void i_ora_abs(emustate* emu, abs_t opr) {

}

// PHA instruction
// PHP insturction

void i_php(emustate* emu) {
    emu->memory[0x100 + emu->sp++] = emu->sr;
}

// PLA instruction
// PLP instruction
// ROL instruction
// ROR instruction
// RTI instruction
// RTS instruction
// SBC instruction

void g_sbc(emustate* emu, uint8_t opr) {
    int16_t res = emu->a - opr - CHECK(emu->sr, FLAG_C); //TODO check this is correct
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
}

// SEC instruction
// SED instruction
// SEI instruction
// STA instruction
// STX instruction
// STY instruction
// TAX instruction
// TAY instruction
// TSX instruction
// TXA instruction
// TXS instruction
// TYA instruction