#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "types.h"
#include "emustate.h"
// https://en.wikipedia.org/wiki/WDC_65C02
// https://www.masswerk.at/6502/6502_instruction_set.html
// first part is documenting all instructions



/* *** NAMING CONVENTION ***
cycles_t i_opc_param1_param2_param3(type p1, type p2, ...);

most opcodes can have multiple different parameters so it's important to differentiate them
going to use a similar convnetion used on the masswerk website linked above

A     ... Accumulator         ... OPC A       - operand is AC (implied single byte instruction)
abs   ... absolute            ... OPC $LLHH   - operand is address $HHLL *
abs,X ... absolute, X-indexed ... OPC $LLHH,X - operand is address; effective address is address incremented by X with carry **
abs,Y ... absolute, Y-indexed ... OPC $LLHH,Y - operand is address; effective address is address incremented by Y with carry **
imd	  ... immediate	          ... OPC #$BB    - operand is byte BB
impl  ... implied             ... OPC         - operand implied
ind   ... indirect            ... OPC ($LLHH) - operand is address; effective address is contents of word at address: C.w($HHLL)
X,ind ... X-indexed, indirect ... OPC ($LL,X) - operand is zeropage address; effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
ind,Y ... indirect, Y-indexed ... OPC ($LL),Y - operand is zeropage address; effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
rel   ... relative            ... OPC $BB     - branch target is PC + signed offset BB ***
zpg   ... zeropage            ... OPC $LL     - operand is zeropage address (hi-byte is zero, address = $00LL)
zpg,X ... zeropage, X-indexed ... OPC $LL,X   - operand is zeropage address; effective address is address incremented by X without carry **
zpg,  ... zeropage, Y-indexed ... OPC $LL,Y   - operand is zeropage address; effective address is address incremented by Y without carry **

*
    16-bit address words are little endian, lo(w)-byte first, followed by the hi(gh)-byte.
    (An assembler will use a human readable, big-endian notation as in $HHLL.)
**
    The available 16-bit address space is conceived as consisting of pages of 256 bytes each, with
    address hi-bytes represententing the page index. An increment with carry may affect the hi-byte
    and may thus result in a crossing of page boundaries, adding an extra cycle to the execution.
    Increments without carry do not affect the hi-byte of an address and no page transitions do occur.
    Generally, increments of 16-bit addresses include a carry, increments of zeropage addresses don't.
    Notably this is not related in any way to the state of the carry bit of the accumulator.
***
    Branch offsets are signed 8-bit values, -128 ... +127, negative offsets in two's complement.
    Page transitions may occur and add an extra cycle to the exucution.

^^^ all copied from the masswerk site
*/

/*
BRK - break
OPC: $00
OPR: implied
*/
cycles_t i_brk(emustate* emu);
const instr_info s_brk = { "BRK", 0x00, Implied, i_brk};

/*
ORA - or with accumulator
OPC: $01
OPR: X-indexed, indirect
*/
cycles_t i_ora_indr_x(emustate* emu, indr_t opr);
const instr_info s_ora_indr_x = { "ORA", 0x01, Indirect, i_ora_indr_x };

/*
RESERVED
OPC: $02
*/

/*
RESERVED
OPC: $03
*/

/*
RESERVED
OPC: $04
*/

/*
ORA - or with accumulator
OPC: $05
OPR: immediate
*/
cycles_t i_ora_zpg(emustate* emu, zpg_t opr);
const instr_info s_ora_zpg = { "ORA", 0x05, Immediate, i_ora_zpg };

/*
ASL - arithmetic shift left
OPC: $06
OPR: zero-page
*/
cycles_t i_asl_zpg(emustate* emu, zpg_t opr);
const instr_info s_asl_zpg = { "ASL", 0x06, Zeropage, i_asl_zpg };

/*
RESERVED
OPC: $07
*/

/*
PHP - push processor status onto stack
OPC: $08
OPR: implied
*/
cycles_t i_php(emustate* emu);
const instr_info s_php = { "PHP", 0x08, Implied, i_php };

/*
ORA - or with accumulator
OPC: $09
OPR: immediate
*/
cycles_t i_ora_imd(emustate* emu, imd_t opr);
const instr_info s_ora_imd = { "ORA", 0x09, Immediate, i_ora_imd };

/*
ASL - artihmetic shift left
OPC: $0A
OPR: accumulator
*/
cycles_t i_asl_a(emustate* emu);
const instr_info s_asl_a = { "ASL", 0x0A, Implied, i_asl_a };

/*
RESERVED
OPC: $0B
*/

/*
RESERVED
OPC: $0C
*/

/*
ORA - or with accumulator
OPC: $0D
OPR: absolute
*/
cycles_t i_ora_abs(emustate* emu, abs_t opr);
const instr_info s_ora_abs = { "ORA", 0x0D, Absolute, i_ora_abs };

/*
ASL - arithmetic shift left
OPC: $0E
OPR: absolute
*/
cycles_t i_asl_abs(emustate* emu, abs_t opr);
const instr_info s_asl_abs = { "ASL", 0x0E, Absolute, i_asl_abs };

/*
RESERVED
OPC: $0F
*/

/*
BPL - branch on plus
OPC: $10
OPR: relative
*/
cycles_t i_bpl_rel(emustate* emu, rel_t opr);
const instr_info s_bpl_rel = { "BPL", 0x10, Relative, i_bpl_rel };

/*
ORA - or with accumulator
OPC: $11
OPR: indirect, Y-indexed
*/
cycles_t i_ora_indr_y(emustate* emu, indr_t opr);
const instr_info s_ora_indr_y = { "ORA", 0x11, Indirect, i_ora_indr_y };

/*
RESERVED
OPC: $12
*/

/*
RESERVED
OPC: $13
*/

/*
RESERVED
OPC: $14
*/

/*
ORA - or with accumulator
OPC: $15
OPR: zero-page, X-indexed
*/
cycles_t i_ora_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_ora_zpg_x = { "ORA", 0x15, Zeropage, i_ora_zpg_x };

/*
ASL - arithmetic shift left
OPC: $16
OPR: zero-page, X-indexed
*/
cycles_t i_asl_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_asl_zpg_x = { "ASL", 0x16, Zeropage, i_asl_zpg_x };

/*
RESERVED
OPC: $17
*/

/*
CLC - clear carry
OPC: $18
OPR: implied
*/
cycles_t i_clc(emustate* emu);
const instr_info s_clc = { "CLC", 0x18, Implied, i_clc };

/*
ORA - or with accumulator
OPC: $19
OPR: absolute, Y-indexed
*/
cycles_t i_ora_abs_y(emustate* emu, abs_t opr);
const instr_info s_ora_abs_y = { "ORA", 0x19, Absolute, i_ora_abs_y };

/*
RESERVED
OPC: $1A
*/

/*
RESERVED
OPC: $1B
*/

/*
RESERVED
OPC: $1C
*/

/*
ORA - or with accumulator
OPC: $1D
OPR: absolute, X-indexed
*/
cycles_t i_ora_abs_x(emustate* emu, abs_t opr);
const instr_info s_ora_abs_x = { "ORA", 0x1D, Absolute, i_ora_abs_x };

/*
ASL - arithmetic shift left
OPC: $1E
OPR: absolute, X-indexed
*/
cycles_t i_asl_abs_x(emustate* emu, abs_t opr);
const instr_info s_asl_abs_x = { "ASL", 0x1E, Absolute, i_asl_abs_x };

/*
JSR - jump subroutine
OPC: $20
OPR: absolute
*/
cycles_t i_jsr_abs(emustate* emu, abs_t opr);
const instr_info s_jsr_abs = { "JSR", 0x20, Absolute, i_jsr_abs };

/*
AND - and
OPC: $21
OPR: X-indexed, indirect
*/
cycles_t i_and_indr_x(emustate* emu, indr_t opr);
const instr_info s_and_indr_x = { "AND", 0x21, Indirect, i_and_indr_x };

/*
RESERVED
OPC: $22
*/

/*
RESERVED
OPC: $23
*/

/*
BIT - bit test
OPC: $24
OPR: zero-page
*/
cycles_t i_bit_zpg(emustate* emu, zpg_t opr);
const instr_info s_bit_zpg = { "BIT", 0x24, Zeropage, i_bit_zpg };

/*
AND - and
OPC: $25
OPR: zero-page
*/
cycles_t i_and_zpg(emustate* emu, zpg_t opr);
const instr_info s_and_zpg = { "AND", 0x25, Zeropage, i_and_zpg };

/*
ROL - rotate left
OPC: $26
OPR: zero-page
*/
cycles_t i_rol_zpg(emustate* emu, zpg_t opr);
const instr_info s_rol_zpg = { "ROL", 0x26, Zeropage, i_rol_zpg };

/*
RESERVED
OPC: $27
*/

/*
PLP - pull processor status from stack
OPC: $28
OPR: implied
*/
cycles_t i_plp(emustate* emu);
const instr_info s_plp = { "PLP", 0x28, Implied, i_plp };

/*
AND - and
OPC: $29
OPR: immediate
*/
cycles_t i_and_imd(emustate* emu, imd_t opr);
const instr_info s_and_imd = { "AND", 0x29, Immediate, i_and_imd };

/*
ROL - rotate left
OPC: $2A
OPR: accumulator
*/
cycles_t i_rol_a(emustate* emu);
const instr_info s_rol_a = { "ROL", 0x2A, Implied, i_rol_a };

/*
RESERVED
OPC: $2B
*/

/*
BIT - bit test
OPC: $2C
OPR: absolute
*/
cycles_t i_bit_abs(emustate* emu, abs_t opr);
const instr_info s_bit_abs = { "BIT", 0x2C, Absolute, i_bit_abs };

/*
AND - and
OPC: $2D
OPR: absolute
*/
cycles_t i_and_abs(emustate* emu, abs_t opr);
const instr_info s_and_abs = { "AND", 0x2D, Absolute, i_and_abs };

/*
ROL - rotate left
OPC: $2E
OPR: absolute
*/
cycles_t i_rol_abs(emustate* emu, abs_t opr);
const instr_info s_rol_abs = { "ROL", 0x2E, Absolute, i_rol_abs };

/*
RESERVED
OPC: $2F
*/

/*
BMI - branch on minus
OPC: $30
OPR: relative
*/
cycles_t i_bmi_rel(emustate* emu, rel_t opr);
const instr_info s_bmi_rel = { "BMI", 0x30, Relative, i_bmi_rel };

/*
AND - and
OPC: $31
OPR: indirect, Y-indexed
*/
cycles_t i_and_indr_y(emustate* emu, indr_t opr);
const instr_info s_and_indr_y = { "AND", 0x31, Indirect, i_and_indr_y };


/*
RESERVED
OPC: $32
*/

/*
RESERVED
OPC: $33
*/

/*
RESERVED
OPC: $34
*/

/*
AND - and
OPC: $35
OPR: zero-page, X-indexed
*/
cycles_t i_and_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_and_zpg_x = { "AND", 0x35, Zeropage, i_and_zpg_x };

/*
ROL - rotate left
OPC: $36
OPR: zero-page, X-indexed
*/
cycles_t i_rol_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_rol_zpg_x = { "ROL", 0x36, Zeropage, i_rol_zpg_x };

/*
RESERVED
OPC: $37
*/

/*
SEC - set carry
OPC: $38
OPR: implied
*/
cycles_t i_sec(emustate* emu);
const instr_info s_sec = { "SEC", 0x38, Implied, i_sec };

/*
AND - and
OPC: $39
OPR: absolute, Y-indexed
*/
cycles_t i_and_abs_y(emustate* emu, abs_t opr);
const instr_info s_and_abs_y = { "AND", 0x39, Absolute, i_and_abs_y };

/*
RESERVED
OPC: $3A
*/

/*
RESERVED
OPC: $3B
*/

/*
RESERVED
OPC: $3C
*/

/*
AND - and
OPC: $3D
OPR: absolute, X-indexed
*/
cycles_t i_and_abs_x(emustate* emu, abs_t opr);
const instr_info s_and_abs_x = { "AND", 0x3D, Absolute, i_and_abs_x };

/*
ROL - rotate left
OPC: $3E
OPR: absolute, X-indexed
*/
cycles_t i_rol_abs_x(emustate* emu, abs_t opr);
const instr_info s_rol_abs_x = { "ROL", 0x3E, Absolute, i_rol_abs_x };

/*
RESERVED
OPC: $3F
*/

/*
RTI - return from interrupt
OPC: $40
OPR: implied
*/
cycles_t i_rti(emustate* emu);
const instr_info s_rti = { "RTI", 0x40, Implied, i_rti };

/*
EOR - exclusive or with accumulator
OPC: $41
OPR: X-indexed, indirect
*/
cycles_t i_eor_indr_x(emustate* emu, indr_t opr);
const instr_info s_eor_indr_x = { "EOR", 0x41, Indirect, i_eor_indr_x };

/*
RESERVED
OPC: $42
*/

/*
RESERVED
OPC: $43
*/

/*
RESERVED
OPC: $44
*/

/*
EOR - exclusive or with accumulator
OPC: $45
OPR: zero-page
*/
cycles_t i_eor_zpg(emustate* emu, zpg_t opr);
const instr_info s_eor_zpg = { "EOR", 0x45, Zeropage, i_eor_zpg };

/*
LSR - logical shift right
OPC: $46
OPR: zero-page
*/
cycles_t i_lsr_zpg(emustate* emu, zpg_t opr);
const instr_info s_lsr_zpg = { "LSR", 0x46, Zeropage, i_lsr_zpg };

/*
RESERVED
OPC: $47
*/

/*
PHA - push accumulator onto stack
OPC: $48
OPR: implied
*/
cycles_t i_pha(emustate* emu);
const instr_info s_pha = { "PHA", 0x48, Implied, i_pha };

/*
EOR - exclusive or
OPC: $49
OPR: immediate
*/
cycles_t i_eor_imd(emustate* emu, imd_t opr);
const instr_info s_eor_imd = { "EOR", 0x49, Immediate, i_eor_imd };

/*
LSR - logical shift right
OPC: $4A
OPR: accumulator
*/
cycles_t i_lsr_a(emustate* emu);
const instr_info s_lsr_a = { "LSR", 0x4A, Implied, i_lsr_a };

/*
RESERVED
OPC: $4B
*/

/*
JMP - jump
OPC: $4C
OPR: absolute
*/
cycles_t i_jmp_abs(emustate* emu, abs_t opr);
const instr_info s_jmp_abs = { "JMP", 0x4C, Absolute, i_jmp_abs };

/*
EOR - exclusive or
OPC: $4D
OPR: absolute
*/
cycles_t i_eor_abs(emustate* emu, abs_t opr);
const instr_info s_eor_abs = { "EOR", 0x4D, Absolute, i_eor_abs };

/*
LSR - logical shift right
OPC: $4E
OPR: absolute
*/
cycles_t i_lsr_abs(emustate* emu, abs_t opr);
const instr_info s_lsr_abs = { "LSR", 0x4E, Absolute, i_lsr_abs };

/*
RESERVED
OPC: $4F
*/

/*
BVC - branch on overflow clear
OPC: $50
OPR: relative
*/
cycles_t i_bvc_rel(emustate* emu, rel_t opr);
const instr_info s_bvc_rel = { "BVC", 0x50, Relative, i_bvc_rel };

/*
EOR - exclusive or
OPC: $51
OPR: indirect, Y-indexed
*/
cycles_t i_eor_indr_y(emustate* emu, indr_t opr);
const instr_info s_eor_indr_y = { "EOR", 0x51, Indirect, i_eor_indr_y };

/*
RESERVED
OPC: $52
*/

/*
RESERVED
OPC: $53
*/

/*
RESERVED
OPC: $54
*/

/*
EOR - exclusive or
OPC: $55
OPR: zero-page, X-indexed
*/
cycles_t i_eor_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_eor_zpg_x = { "EOR", 0x55, Zeropage, i_eor_zpg_x };

/*
LSR - logical shift right
OPC: $56
OPR: zero-page, X-indexed
*/
cycles_t i_lsr_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_lsr_zpg_x = { "LSR", 0x56, Zeropage, i_lsr_zpg_x };

/*
RESERVED
OPC: $57
*/

/*
CLI - clear interrupt disable
OPC: $58
OPR: implied
*/
cycles_t i_cli(emustate* emu);
const instr_info s_cli = { "CLI", 0x58, Implied, i_cli };

/*
EOR - exclusive or
OPC: $59
OPR: absolute, Y-indexed
*/
cycles_t i_eor_abs_y(emustate* emu, abs_t opr);
const instr_info s_eor_abs_y = { "EOR", 0x59, Absolute, i_eor_abs_y };

/*
RESERVED
OPC: $5A
*/

/*
RESERVED
OPC: $5B
*/

/*
RESERVED
OPC: $5C
*/

/*
EOR - exclusive or
OPC: $5D
OPR: absolute, X-indexed
*/
cycles_t i_eor_abs_x(emustate* emu, abs_t opr);
const instr_info s_eor_abs_x = { "EOR", 0x5D, Absolute, i_eor_abs_x };

/*
LSR - logical shift right
OPC: $5E
OPR: absolute, X-indexed
*/
cycles_t i_lsr_abs_x(emustate* emu, abs_t opr);
const instr_info s_lsr_abs_x = { "LSR", 0x5E, Absolute, i_lsr_abs_x };

/*
RESERVED
OPC: $5F
*/

/*
RTS - return from subroutine
OPC: $60
OPR: implied
*/
cycles_t i_rts(emustate* emu);
const instr_info s_rts = { "RTS", 0x60, Implied, i_rts };

/*
ADC - add with carry
OPC: $61
OPR: X-indexed, indirect
*/
cycles_t i_adc_indr_x(emustate* emu, indr_t opr);
const instr_info s_adc_indr_x = { "ADC", 0x61, Indirect, i_adc_indr_x };

/*
RESERVED
OPC: $62
*/

/*
RESERVED
OPC: $63
*/

/*
RESERVED
OPC: $64
*/

/*
ADC - add with carry
OPC: $65
OPR: zero-page
*/
cycles_t i_adc_zpg(emustate* emu, zpg_t opr);
const instr_info s_adc_zpg = { "ADC", 0x65, Zeropage, i_adc_zpg };

/*
ROR - rotate right
OPC: $66
OPR: zero-page
*/
cycles_t i_ror_zpg(emustate* emu, zpg_t opr);
const instr_info s_ror_zpg = { "ROR", 0x66, Zeropage, i_ror_zpg };

/*
RESERVED
OPC: $67
*/

/*
PLA - pull accumulator from stack
OPC: $68
OPR: implied
*/
cycles_t i_pla(emustate* emu);
const instr_info s_pla = { "PLA", 0x68, Implied, i_pla };

/*
ADC - add with carry
OPC: $69
OPR: immediate
*/
cycles_t i_adc_imd(emustate* emu, imd_t opr);
const instr_info s_adc_imd = { "ADC", 0x69, Immediate, i_adc_imd };

/*
ROR - rotate right
OPC: $6A
OPR: accumulator
*/
cycles_t i_ror_a(emustate* emu);
const instr_info s_ror_a = { "ROR", 0x6A, Implied, i_ror_a };

/*
RESERVED
OPC: $6B
*/

/*
JMP - jump
OPC: $6C
OPR: indirect
*/
cycles_t i_jmp_indr(emustate* emu, indr_t opr);
const instr_info s_jmp_indr = { "JMP", 0x6C, Indirect, i_jmp_indr };

/*
ADC - add with carry
OPC: $6D
OPR: absolute
*/
cycles_t i_adc_abs(emustate* emu, abs_t opr);
const instr_info s_adc_abs = { "ADC", 0x6D, Absolute, i_adc_abs };

/*
ROR - rotate right
OPC: $6E
OPR: absolute
*/
cycles_t i_ror_abs(emustate* emu, abs_t opr);
const instr_info s_ror_abs = { "ROR", 0x6E, Absolute, i_ror_abs };

/*
RESERVED
OPC: $6F
*/

/*
BVS - branch on overflow set
OPC: $70
OPR: relative
*/
cycles_t i_bvs_rel(emustate* emu, rel_t opr);
const instr_info s_bvs_rel = { "BVS", 0x70, Relative, i_bvs_rel };

/*
ADC - add with carry
OPC: $71
OPR: indirect, Y-indexed
*/
cycles_t i_adc_indr_y(emustate* emu, indr_t opr);
const instr_info s_adc_indr_y = { "ADC", 0x71, Indirect, i_adc_indr_y };

/*
RESERVED
OPC: $72
*/

/*
RESERVED
OPC: $73
*/

/*
RESERVED
OPC: $74
*/

/*
ADC - add with carry
OPR: $75
OPC: zero-page, X-indexed
*/

cycles_t i_adc_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_adc_zpg_x = { "ADC", 0x75, Zeropage, i_adc_zpg_x };

/*
ROR - rotate right
OPR: $76
OPC: zero-page, X-indexed
*/
cycles_t i_ror_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_ror_zpg_x = { "ROR", 0x76, Zeropage, i_ror_zpg_x };

/*
RESERVED
OPC: $77
*/

/*
SEI - system interrupt disable
OPC: $78
OPR: implied
*/
cycles_t i_sei(emustate* emu);
const instr_info s_sei = { "SEI", 0x78, Implied, i_sei };

/*
ADC - add with carry
OPC: $79
OPR: absolute, Y-indexed
*/
cycles_t i_adc_abs_y(emustate* emu, abs_t opr);
const instr_info s_adc_abs_y = { "ADC", 0x79, Absolute, i_adc_abs_y };

/*
RESERVED
OPC: $7A
*/

/*
RESERVED
OPC: $7B
*/

/*
RESERVED
OPC: $7C
*/

/*
ADC - add with carry
OPC: $7D
OPR: absolute, X-indexed
*/
cycles_t i_adc_abs_x(emustate* emu, abs_t opr);
const instr_info s_adc_abs_x = { "ADC", 0x7D, Absolute, i_adc_abs_x };

/*
ROR - rotate right
OPC: $7E
OPR: absolute, X-indexed
*/
cycles_t i_ror_abs_x(emustate* emu, abs_t opr);
const instr_info s_ror_abs_x = { "ROR", 0x7E, Absolute, i_ror_abs_x };

/*
RESERVED
OPC: $7F
*/

/*
RESERVED
OPC: $80
*/

/*
STA - store accumulator
OPC: $81
OPR: X-indexed, indirect
*/
cycles_t i_sta_indr_x(emustate* emu, indr_t opr);
const instr_info s_sta_indr_x = { "STA", 0x81, Indirect, i_sta_indr_x };

/*
RESERVED
OPC: $82
*/

/*
RESERVED
OPC: $83
*/

/*
STY - store Y
OPC: $84
OPR: zero-page
*/
cycles_t i_sty_zpg(emustate* emu, zpg_t opr);
const instr_info s_sty_zpg = { "STY", 0x84, Zeropage, i_sty_zpg };

/*
STA - store accumulator
OPC: $85
OPR: zero-page
*/
cycles_t i_sta_zpg(emustate* emu, zpg_t opr);
const instr_info s_sta_zpg = { "STA", 0x85, Zeropage, i_sta_zpg };

/*
STX - store X
OPC: $86
OPR: zero-page
*/
cycles_t i_stx_zpg(emustate* emu, zpg_t opr);
const instr_info s_stx_zpg = { "STX", 0x86, Zeropage, i_stx_zpg };

/*
RESERVED
OPC: $87
*/

/*
DEY - decrement Y
OPC: $88
OPR: implied
*/
cycles_t i_dey(emustate* emu);
const instr_info s_dey = { "DEY", 0x88, Implied, i_dey };

/*
RESERVED
OPC: $89
*/

/*
TXA - transfer X to accumulator
OPC: $8A
OPR: implied
*/
cycles_t i_txa(emustate* emu);
const instr_info s_txa = { "TXA", 0x8A, Implied, i_txa };

/*
RESERVED
OPC: $8B
*/

/*
STY - store Y
OPC: $8C
OPR: absolute
*/
cycles_t i_sty_abs(emustate* emu, abs_t opr);
const instr_info s_sty_abs = { "STY", 0x8C, Absolute, i_sty_abs };

/*
STA - store accumulator
OPR: $8D
OPR: absolute
*/
cycles_t i_sta_abs(emustate* emu, abs_t opr);
const instr_info s_sta_abs = { "STA", 0x8D, Absolute, i_sta_abs };

/*
STX - store X
OPR: $8E
OPR: absolute
*/
cycles_t i_stx_abs(emustate* emu, abs_t opr);
const instr_info s_stx_abs = { "STX", 0x8E, Absolute, i_stx_abs };

/*
RESERVED
OPC: $8F
*/

/*
BCC - branch on carry clear
OPC: $90
OPR: relative
*/
cycles_t i_bcc_rel(emustate* emu, rel_t opr);
const instr_info s_bcc_rel = { "BCC", 0x90, Relative, i_bcc_rel };

/*
STA - store accumulator
OPC: $91
OPR: indirect, Y-indexed
*/
cycles_t i_sta_indr_y(emustate* emu, indr_t opr);
const instr_info s_sta_indr_y = { "STA", 0x91, Indirect, i_sta_indr_y };

/*
RESERVED
OPC: $92
*/

/*
RESERVED
OPC: $93
*/

/*
STY - store Y
OPC: $94
OPR: zero-page, X-indexed
*/
cycles_t i_sty_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_sty_zpg_x = { "STY", 0x94, Zeropage, i_sty_zpg_x };

/*
STA - store accumulator
OPC: $95
OPR: zero-page, X-indexed
*/
cycles_t i_sta_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_sta_zpg_x = { "STA", 0x95, Zeropage, i_sta_zpg_x };

/*
STX - store X
OPC: $96
OPR: zero-page, Y-indexed
*/
cycles_t i_stx_zpg_y(emustate* emu, zpg_t opr);
const instr_info s_stx_zpg_y = { "STX", 0x96, Zeropage, i_stx_zpg_y };

/*
RESERVED
OPC: $97
*/

/*
TYA - transfer Y to accumulator
OPC: $98
OPR: implied
*/
cycles_t i_tya(emustate* emu);
const instr_info s_tya = { "TYA", 0x98, Implied, i_tya };

/*
STA - store accumulator
OPC: $99
OPR: absolute, Y-indexed
*/
cycles_t i_sta_abs_y(emustate* emu, abs_t opr);
const instr_info s_sta_abs_y = { "STA", 0x99, Absolute, i_sta_abs_y };

/*
TXS - transfer X to stack pointer
OPC: $9A
OPR: implied
*/
cycles_t i_txs(emustate* emu);
const instr_info s_txs = { "TXA", 0x9A, Implied, i_txs };

/*
RESERVED
OPC: $9B
*/

/*
RESERVED
OPC: $9C
*/

/*
STA - store accumulator
OPC: $9D
OPR: absolute, X-indexed
*/
cycles_t i_sta_abs_x(emustate* emu, abs_t opr);
const instr_info s_sta_abs_x = { "STA", 0x9D, Absolute, i_sta_abs_x };

/*
RESERVED
OPC: $9E
*/

/*
RESERVED
OPC: $9F
*/

/*
LDY - load Y
OPC: $A0
OPR: immediate
*/
cycles_t i_ldy_imd(emustate* emu, imd_t opr);
const instr_info s_ldy_imd = { "LDY", 0xA0, Immediate, i_ldy_imd };

/*
LDA - load A
OPC: $A1
OPR: X-indexed, indirect
*/
cycles_t i_lda_indr_x(emustate* emu, indr_t opr);
const instr_info s_lda_indr_x = { "LDA", 0xA1, Indirect, i_lda_indr_x };

/*
LDX - load X
OPC: $A2
OPR: immediate
*/
cycles_t i_ldx_imd(emustate* emu, imd_t opr);
const instr_info s_ldx_imd = { "LDX", 0xA2, Immediate, i_ldx_imd };

/*
RESERVED
OPC: $A3
*/

/*
LDY - load Y
OPC: $A4
OPR: zero-page
*/
cycles_t i_ldy_zpg(emustate* emu, zpg_t opr);
const instr_info s_ldy_zpg = { "LDY", 0xA4, Zeropage, i_ldy_zpg };

/*
LDA - load accumulator
OPC: $A5
OPR: zero-page
*/
cycles_t i_lda_zpg(emustate* emu, zpg_t opr);
const instr_info s_lda_zpg = { "LDA", 0xA5, Zeropage, i_lda_zpg };

/*
LDX - load X
OPC: $A6
OPR: zero-page
*/
cycles_t i_ldx_zpg(emustate* emu, zpg_t opr);
const instr_info s_ldx_zpg = { "LDX", 0xA6, Zeropage, i_ldx_zpg };

/*
RESERVED
OPC: $A7
*/

/*
TAY - transfer accumulator to Y
OPC: $A8
OPR: implied
*/
cycles_t i_tay(emustate* emu);
const instr_info s_tay = { "TAY", 0xA8, Implied, i_tay };

/*
LDA - load accumulator
OPC: $A9
OPR: immediate
*/
cycles_t i_lda_imd(emustate* emu, imd_t opr);
const instr_info s_lda_imd = { "LDA", 0xA9, Immediate, i_lda_imd };

/*
TAX - transfer accumulator to X
OPC: $AA
OPR: implied
*/
cycles_t i_tax(emustate* emu);
const instr_info s_tax = { "TAX", 0xAA, Implied, i_tax };

/*
RESERVED
OPC: $AB
*/

/*
LDY - load Y
OPC: $AC
OPR: absolute
*/
cycles_t i_ldy_abs(emustate* emu, abs_t opr);
const instr_info s_ldy_abs = { "LDY", 0xAC, Absolute, i_ldy_abs };

/*
LDA - load accumulator
OPC: $AD
OPR: absolute
*/
cycles_t i_lda_abs(emustate* emu, abs_t opr);
const instr_info s_lda_abs = { "LDA", 0xAD, Absolute, i_lda_abs };

/*
LDX - load X
OPC: $AE
OPR: absolute
*/
cycles_t i_ldx_abs(emustate* emu, abs_t opr);
const instr_info s_ldx_abs = { "LDX", 0xAE, Absolute, i_ldx_abs };

/*
RESERVED
OPC: $AF
*/

/*
BCS - branch on carry set
OPC: $B0
OPR: relative
*/
cycles_t i_bcs_rel(emustate* emu, rel_t opr);
const instr_info s_bcs_rel = { "BCS", 0xB0, Relative, i_bcs_rel };

/*
LDA - load accumulator
OPC: $B1
OPR: indirect, Y-indexed
*/
cycles_t i_lda_indr_y(emustate* emu, indr_t opr);
const instr_info s_lda_indr_y = { "LDA", 0xB1, Indirect, i_lda_indr_y };

/*
RESERVED
OPC: $B2
*/

/*
RESERVED
OPC: $B3
*/

/*
LDY - load Y
OPC: $B4
OPR: zero-page, X-indexed
*/
cycles_t i_ldy_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_ldy_zpg_x = { "LDY", 0xB4, Zeropage, i_ldy_zpg_x };

/*
LDA - load accumulator
OPC: $B5
OPR: zero-page, X-indexed
*/
cycles_t i_lda_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_lda_zpg_x = { "LDA", 0xB5, Zeropage, i_lda_zpg_x };

/*
LDX - load X
OPC: $B6
OPR: zero-page, Y-indexed
*/
cycles_t i_ldx_zpg_y(emustate* emu, zpg_t opr);
const instr_info s_ldx_zpg_y = { "LDX", 0xB6, Zeropage, i_ldx_zpg_y };

/*
RESERVED
OPC: $B7
*/

/*
CLV - clear overflow
OPC: $B8
OPR: implied
*/
cycles_t i_clv(emustate* emu);
const instr_info s_clv = { "CLV", 0xB8, Implied, i_clv };

/*
LDA - load accumulator
OPC: $B9
OPR: absolute, Y-indexed
*/
cycles_t i_lda_abs_y(emustate* emu, abs_t opr);
const instr_info s_lda_abs_y = { "LDA", 0xB9, Absolute, i_lda_abs_y };

/*
TSX - transfer stack pointer to X
OPC: $BA
OPR: implied
*/
cycles_t i_tsx(emustate* emu);
const instr_info s_tsx = { "TSX", 0xBA, Implied, i_tsx };

/*
RESERVED
OPC: $BB
*/

/*
LDY - load Y
OPC: $BC
OPR: absolute, X-indexed
*/
cycles_t i_ldy_abs_x(emustate* emu, abs_t opr);
const instr_info s_ldy_abs_x = { "LDY", 0xBC, Absolute, i_ldy_abs_x };

/*
LDA - load accumulator
OPC: $BD
OPR: absolute, X-indexed
*/
cycles_t i_lda_abs_x(emustate* emu, abs_t opr);
const instr_info s_lda_abs_x = { "LDA", 0xBD, Absolute, i_lda_abs_x };

/*
LDX - load X
OPC: $BE
OPR: absolute, Y-indexed
*/
cycles_t i_ldx_abs_y(emustate* emu, abs_t opr);
const instr_info s_ldx_abs_y = { "LDX", 0xBE, Absolute, i_ldx_abs_y };

/*
RESERVED
OPC: $BF
*/

/*
CPY - compare with Y
OPC: $C0
OPR: immediate
*/
cycles_t i_cpy_imd(emustate* emu, imd_t opr);
const instr_info s_cpy_imd = { "CPY", 0xC0, Immediate, i_cpy_imd };

/*
CMP - compare with accumulator
OPC: $C1
OPR: X-indexed, indirect
*/
cycles_t i_cmp_indr_x(emustate* emu, indr_t opr);
const instr_info s_cmp_indr_x = { "CMP", 0xC1, Indirect, i_cmp_indr_x };
/*
RESERVED
OPC: $C2
*/

/*
RESERVED
OPC: $C3
*/

/*
CPY - compare with Y
OPC: $C4
OPR: zero-page
*/
cycles_t i_cpy_zpg(emustate* emu, zpg_t opr);
const instr_info s_cpy_zpg = { "CPY", 0xC4, Zeropage, i_cpy_zpg };

/*
CMP - compare with accumulator
OPC: $C5
OPR: zero-page
*/
cycles_t i_cmp_zpg(emustate* emu, zpg_t opr);
const instr_info s_cmp_zpg = { "CMP", 0xC5, Zeropage, i_cmp_zpg };

/*
DEC - decrement
OPC: $C6
OPR: zero-page
*/
cycles_t i_dec_zpg(emustate* emu, zpg_t opr);
const instr_info s_dec_zpg = { "DEC", 0xC6, Zeropage, i_dec_zpg };

/*
RESERVED
OPC: $C7
*/

/*
INY - increment Y
OPC: $C8
OPR: implied
*/
cycles_t i_iny(emustate* emu);
const instr_info s_iny = { "INY", 0xC8, Implied, i_iny };

/*
CMP - compare with accumulator
OPC: $C9
OPR: immediate
*/
cycles_t i_cmp_imd(emustate* emu, imd_t opr);
const instr_info s_cmp_imd = { "CMP", 0xC9, Immediate, i_cmp_imd };

/*
DEX - decrement X
OPC: $CA
OPR: implied
*/
cycles_t i_dex(emustate* emu);
const instr_info s_dex = { "DEX", 0xCA, Implied, i_dex };

/*
RESERVED
OPC: $CB
*/

/*
CPY - compare with Y
OPC: $CC
OPR: absolute
*/
cycles_t i_cpy_abs(emustate* emu, abs_t opr);
const instr_info s_cpy_abs = { "CPY", 0xCC, Absolute, i_cpy_abs };

/*
CMP - compare with accumulator
OPC: $CD
OPR: absolute
*/
cycles_t i_cmp_abs(emustate* emu, abs_t opr);
const instr_info s_cmp_abs = { "CMP", 0xCD, Absolute, i_cmp_abs };

/*
DEC - decrement
OPC: $CE
OPR: absolute
*/
cycles_t i_dec_abs(emustate* emu, abs_t opr);
const instr_info s_dec_abs = { "DEC", 0xCE, Absolute, i_dec_abs };

/*
RESERVED
OPC: $CF
*/

/*
BNE - branch not equal
OPC: $D0
OPR: relative
*/
cycles_t i_bne_rel(emustate* emu, rel_t opr);
const instr_info s_bne_rel = { "BNE", 0xD0, Relative, i_bne_rel };

/*
CMP - compare with accumulator
OPC: $D1
OPR: indirect, Y-indexed
*/
cycles_t i_cmp_indr_y(emustate* emu, indr_t opr);
const instr_info s_cmp_indr_y = { "CMP", 0xD1, Indirect, i_cmp_indr_y };

/*
RESERVED
OPC: $D2
*/

/*
RESERVED
OPC: $D3
*/

/*
RESERVED
OPC: $D4
*/

/*
CMP - compare with accumulator
OPC: $D5
OPR: zero-page, X-indexed
*/
cycles_t i_cmp_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_cmp_zpg_x = { "CMP", 0xD5, Zeropage, i_cmp_zpg_x };

/*
DEC - decrement
OPC: $D6
OPR: zero-page, X-indexed
*/
cycles_t i_dec_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_dec_zpg_x = { "DEC", 0xD6, Zeropage, i_dec_zpg_x };

/*
RESERVED
OPC: $D7
*/

/*
CLD - clear decimal
OPC: $D8
OPR: implied
*/
cycles_t i_cld(emustate* emu);
const instr_info s_cld = { "CLD", 0xD8, Implied, i_cld };

/*
CMP - compare with accumulator
OPC: $D9
OPR: absolute, Y-indexed
*/
cycles_t i_cmp_abs_y(emustate* emu, abs_t opr);
const instr_info s_cmp_abs_y = { "CMP", 0xD9, Absolute, i_cmp_abs_y };

/*
RESERVED
OPC: $DA
*/

/*
RESERVED
OPC: $DB
*/

/*
RESERVED
OPC: $DC
*/

/*
CMP - compare with accumulator
OPC: $DD
OPR: absolute, X-indexed
*/
cycles_t i_cmp_abs_x(emustate* emu, abs_t opr);
const instr_info s_cmp_abs_x = { "CMP", 0xDD, Absolute, i_cmp_abs_x };

/*
DEC - decrement
OPC: $DE
OPR: absolute, X-indexed
*/
cycles_t i_dec_abs_x(emustate* emu, abs_t opr);
const instr_info s_dec_abs_x = { "DEC", 0xDE, Absolute, i_dec_abs_x };

/*
RESERVED
OPC: $DF
*/

/*
CPX - compare with X
OPC: $E0
OPR: immediate
*/
cycles_t i_cpx_imd(emustate* emu, imd_t opr);
const instr_info s_cpx_imd = { "CPX", 0xE0, Immediate, i_cpx_imd };

/*
SBC - subtract with carry
OPC: $E1
OPR: X-indexed, indirect
*/
cycles_t i_sbc_indr_x(emustate* emu, indr_t opr);
const instr_info s_sbc_indr_x = { "SBC", 0xE1, Indirect, i_sbc_indr_y };

/*
RESERVED
OPC: $E2
*/

/*
RESERVED
OPC: $E3
*/

/*
CPX - compare with X
OPC: $E4
OPR: zero-page
*/
cycles_t i_cpx_zpg(emustate* emu, zpg_t opr);
const instr_info s_cpx_zpg = { "CPX", 0xE4, Zeropage, i_cpx_zpg };

/*
SBC - subtract with carry
OPC: $E5
OPR: zero-page
*/
cycles_t i_sbc_zpg(emustate* emu, zpg_t opr);
const instr_info s_sbc_zpg = { "SBC", 0xE5, Zeropage, i_sbc_zpg };

/*
INC - increment
OPC: $E6
OPR: zero-page
*/
cycles_t i_inc_zpg(emustate* emu, zpg_t opr);
const instr_info s_inc_zpg = { "INC", 0xE6, Zeropage, i_inc_zpg };

/*
RESERVED
OPC: $E7
*/

/*
INX - increment X
OPC: $E8
OPR: implied
*/
cycles_t i_inx(emustate* emu);
const instr_info s_inx = { "INX", 0xE8, Implied, i_inx };

/*
SBC - subtract with carry
OPC: $E9
OPR: immediate
*/
cycles_t i_sbc_imd(emustate* emu, imd_t opr);
const instr_info s_sbc_imd = { "SBC", 0xE9, Immediate, i_sbc_imd };

/*
NOP - no operation
OPC: $EA
OPR: implied
*/
cycles_t i_nop(emustate* emu);
const instr_info s_nop = { "NOP", 0xEA, Implied, i_nop };

/*
RESERVED
OPC: $EB
*/

/*
CPX - compare with X
OPC: $EC
OPR: absolute
*/
cycles_t i_cpx_abs(emustate* emu, abs_t opr);
const instr_info s_cpx_abs = { "CPX", 0xEC, Absolute, i_cpx_abs };

/*
SBC - subtract with carry
OPC: $ED
OPR: absolute
*/
cycles_t i_sbc_abs(emustate* emu, abs_t opr);
const instr_info s_sbc_abs = { "SBC", 0xED, Absolute, i_sbc_abs };

/*
INC - increment
OPC: $EE
OPR: absolute
*/
cycles_t i_inc_abs(emustate* emu, abs_t opr);
const instr_info s_inc_abs = { "INC", 0xEE, Absolute, i_inc_abs };

/*
RESERVED
OPC: $EF
*/

/*
BEQ - branch equal to
OPC: $F0
OPR: relative
*/
cycles_t i_beq_rel(emustate* emu, rel_t opr);
const instr_info s_beq_rel = { "BEQ", 0xF0, Relative, i_beq_rel };

/*
SBC - subtract with carry
OPC: $F1
OPR: indirect, Y-indexed
*/
cycles_t i_sbc_indr_y(emustate* emu, indr_t opr);
const instr_info s_sbc_indr_y = { "SBC", 0xF1, Indirect, i_sbc_indr_y };

/*
RESERVED
OPC: $F2
*/

/*
RESERVED
OPC: $F3
*/

/*
RESERVED
OPC: $F4
*/

/*
SBC - subtract with carry
OPC: $F5
OPR: zero-page, X-indexed
*/
cycles_t i_sbc_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_sbc_zpg_x = { "SBC", 0xF5, Zeropage, i_sbc_zpg_x };

/*
INC - increment
OPC: $F6
OPR: zero-page, X-indexed
*/
cycles_t i_inc_zpg_x(emustate* emu, zpg_t opr);
const instr_info s_inc_zpg_x = { "INC", 0xF6, Zeropage, i_inc_zpg_x };

/*
RESERVED
OPC: $F7
*/

/*
SED - set decimal
OPC: $F8
OPR: implied
*/
cycles_t i_sed(emustate* emu);
const instr_info s_sed = { "SED", 0xF8, Implied, i_sed };

/*
SBC - subtract with carry
OPC: $F9
OPR: absolute, Y-indexed
*/
cycles_t i_sbc_abs_y(emustate* emu, abs_t opr);
const instr_info s_sbc_abs_y = { "SBC", 0xF9, Absolute, i_sbc_abs_y };

/*
RESERVED
OPC: $FA
*/

/*
RESERVED
OPC: $FB
*/

/*
RESERVED
OPC: $FC
*/

/*
SBC - subtract with carry
OPC: $FD
OPR: absolute, X-indexed
*/
cycles_t i_sbc_abs_x(emustate* emu, abs_t opr);
const instr_info s_sbc_abs_x = { "SBC", 0xFD, Absolute, i_sbc_abs_x };

/*
INC - increment
OPC: $FE
OPR: absolute, X-indexed
*/
cycles_t i_inc_abs_x(emustate* emu, abs_t opr);
const instr_info s_inc_abs_x = { "INC", 0xFE, Absolute, i_inc_abs_x };

/*
RESERVED
OPC: $FF
*/

#endif