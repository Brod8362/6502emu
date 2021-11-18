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

/*
ORA - or with accumulator
OPC: $01
OPR: X-indexed, indirect
*/
cycles_t i_ora_indr_x(emustate* emu, indr_t opr);

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

/*
ASL - arithmetic shift left
OPC: $06
OPR: zero-page
*/
cycles_t i_asl_zpg(emustate* emu, zpg_t opr);

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

/*
ORA - or with accumulator
OPC: $09
OPR: immediate
*/
cycles_t i_ora_imd(emustate* emu, imd_t opr);

/*
ASL - artihmetic shift left
OPC: $0A
OPR: accumulator
*/
cycles_t i_asl_a(emustate* emu);

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

/*
ASL - arithmetic shift left
OPC: $0E
OPR: absolute
*/
cycles_t i_asl_abs(emustate* emu, abs_t opr);

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

/*
ORA - or with accumulator
OPC: $11
OPR: indirect, Y-indexed
*/
cycles_t i_ora_indr_y(emustate* emu, indr_t opr);

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

/*
ASL - arithmetic shift left
OPC: $16
OPR: zero-page, X-indexed
*/
cycles_t i_asl_zpg_x(emustate* emu, zpg_t opr);

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

/*
ORA - or with accumulator
OPC: $19
OPR: absolute, Y-indexed
*/
cycles_t i_ora_abs_y(emustate* emu, abs_t opr);

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

/*
ASL - arithmetic shift left
OPC: $1E
OPR: absolute, X-indexed
*/
cycles_t i_asl_abs_x(emustate* emu, abs_t opr);

/*
JSR - jump subroutine
OPC: $20
OPR: absolute
*/
cycles_t i_jsr_abs(emustate* emu, abs_t opr);

/*
AND - and
OPC: $21
OPR: X-indexed, indirect
*/
cycles_t i_and_indr_x(emustate* emu, indr_t opr);

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

/*
AND - and
OPC: $25
OPR: zero-page
*/
cycles_t i_and_zpg(emustate* emu, zpg_t opr);

/*
ROL - rotate left
OPC: $26
OPR: zero-page
*/
cycles_t i_rol_zpg(emustate* emu, zpg_t opr);

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

/*
AND - and
OPC: $29
OPR: immediate
*/
cycles_t i_and_imd(emustate* emu, imd_t opr);

/*
ROL - rotate left
OPC: $2A
OPR: accumulator
*/
cycles_t i_rol_a(emustate* emu);

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

/*
AND - and
OPC: $2D
OPR: absolute
*/
cycles_t i_and_abs(emustate* emu, abs_t opr);

/*
ROL - rotate left
OPC: $2E
OPR: absolute
*/
cycles_t i_rol_abs(emustate* emu, abs_t opr);

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

/*
AND - and
OPC: $31
OPR: indirect, Y-indexed
*/
cycles_t i_and_indr_y(emustate* emu, indr_t opr);


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

/*
ROL - rotate left
OPC: $36
OPR: zero-page, X-indexed
*/
cycles_t i_rol_zpg_x(emustate* emu, zpg_t opr);

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

/*
AND - and
OPC: $39
OPR: absolute, Y-indexed
*/
cycles_t i_and_abs_y(emustate* emu, abs_t opr);

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

/*
ROL - rotate left
OPC: $3E
OPR: absolute, X-indexed
*/
cycles_t i_rol_abs_x(emustate* emu, abs_t opr);

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

/*
EOR - exclusive or with accumulator
OPC: $41
OPR: X-indexed, indirect
*/
cycles_t i_eor_indr_x(emustate* emu, indr_t opr);

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

/*
LSR - logical shift right
OPC: $46
OPR: zero-page
*/
cycles_t i_lsr_zpg(emustate* emu, zpg_t opr);

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

/*
EOR - exclusive or
OPC: $49
OPR: immediate
*/
cycles_t i_eor_imd(emustate* emu, imd_t opr);

/*
LSR - logical shift right
OPC: $4A
OPR: accumulator
*/
cycles_t i_lsr_a(emustate* emu);

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

/*
EOR - exclusive or
OPC: $4D
OPR: absolute
*/
cycles_t i_eor_abs(emustate* emu, abs_t opr);

/*
LSR - logical shift right
OPC: $4E
OPR: absolute
*/
cycles_t i_lsr_abs(emustate* emu, abs_t opr);

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

/*
EOR - exclusive or
OPC: $51
OPR: indirect, Y-indexed
*/
cycles_t i_eor_indr_y(emustate* emu, indr_t opr);

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

/*
LSR - logical shift right
OPC: $56
OPR: zero-page, X-indexed
*/
cycles_t i_lsr_zpg_x(emustate* emu, zpg_t opr);

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

/*
EOR - exclusive or
OPC: $59
OPR: absolute, Y-indexed
*/
cycles_t i_eor_abs_y(emustate* emu, abs_t opr);

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

/*
LSR - logical shift right
OPC: $5E
OPR: absolute, X-indexed
*/
cycles_t i_lsr_abs_x(emustate* emu, abs_t opr);

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

/*
ADC - add with carry
OPC: $61
OPR: X-indexed, indirect
*/
cycles_t i_adc_indr_x(emustate* emu, indr_t opr);

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

/*
ROR - rotate right
OPC: $66
OPR: zero-page
*/
cycles_t i_ror_zpg(emustate* emu, zpg_t opr);

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

/*
ADC - add with carry
OPC: $69
OPR: immediate
*/
cycles_t i_adc_imd(emustate* emu, imd_t opr);

/*
ROR - rotate right
OPC: $6A
OPR: accumulator
*/
cycles_t i_ror_a(emustate* emu);

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

/*
ADC - add with carry
OPC: $6D
OPR: absolute
*/
cycles_t i_adc_abs(emustate* emu, abs_t opr);

/*
ROR - rotate right
OPC: $6E
OPR: absolute
*/
cycles_t i_ror_abs(emustate* emu, abs_t opr);

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

/*
ADC - add with carry
OPC: $71
OPR: indirect, Y-indexed
*/
cycles_t i_adc_indr_y(emustate* emu, indr_t opr);

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

/*
ROR - rotate right
OPR: $76
OPC: zero-page, X-indexed
*/
cycles_t i_ror_zpg_x(emustate* emu, zpg_t opr);

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

/*
ADC - add with carry
OPC: $79
OPR: absolute, Y-indexed
*/
cycles_t i_adc_abs_y(emustate* emu, abs_t opr);

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

/*
ROR - rotate right
OPC: $7E
OPR: absolute, X-indexed
*/
cycles_t i_ror_abs_x(emustate* emu, abs_t opr);

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

/*
STA - store accumulator
OPC: $85
OPR: zero-page
*/
cycles_t i_sta_zpg(emustate* emu, zpg_t opr);

/*
STX - store X
OPC: $86
OPR: zero-page
*/
cycles_t i_stx_zpg(emustate* emu, zpg_t opr);

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

/*
STA - store accumulator
OPR: $8D
OPR: absolute
*/
cycles_t i_sta_abs(emustate* emu, abs_t opr);

/*
STX - store X
OPR: $8E
OPR: absolute
*/
cycles_t i_stx_abs(emustate* emu, abs_t opr);

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

/*
STA - store accumulator
OPC: $91
OPR: indirect, Y-indexed
*/
cycles_t i_sta_indr_y(emustate* emu, indr_t opr);

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

/*
STA - store accumulator
OPC: $95
OPR: zero-page, X-indexed
*/
cycles_t i_sta_zpg_x(emustate* emu, zpg_t opr);

/*
STX - store X
OPC: $96
OPR: zero-page, Y-indexed
*/
cycles_t i_stx_zpg_y(emustate* emu, zpg_t opr);

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

/*
STA - store accumulator
OPC: $99
OPR: absolute, Y-indexed
*/
cycles_t i_sta_abs_y(emustate* emu, abs_t opr);

/*
TXS - transfer X to stack pointer
OPC: $9A
OPR: implied
*/
cycles_t i_txs(emustate* emu);

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

/*
LDA - load A
OPC: $A1
OPR: X-indexed, indirect
*/
cycles_t i_lda_indr_x(emustate* emu, indr_t opr);

/*
LDX - load X
OPC: $A2
OPR: immediate
*/
cycles_t i_ldx_imd(emustate* emu, imd_t opr);

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

/*
LDA - load accumulator
OPC: $A5
OPR: zero-page
*/
cycles_t i_lda_zpg(emustate* emu, zpg_t opr);

/*
LDX - load X
OPC: $A6
OPR: zero-page
*/
cycles_t i_ldx_zpg(emustate* emu, zpg_t opr);

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

/*
LDA - load accumulator
OPC: $A9
OPR: immediate
*/
cycles_t i_lda_imd(emustate* emu, imd_t opr);

/*
TAX - transfer accumulator to X
OPC: $AA
OPR: implied
*/
cycles_t i_tax(emustate* emu);

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

/*
LDA - load accumulator
OPC: $AD
OPR: absolute
*/
cycles_t i_lda_abs(emustate* emu, abs_t opr);

/*
LDX - load X
OPC: $AE
OPR: absolute
*/
cycles_t i_ldx_abs(emustate* emu, abs_t opr);

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

/*
LDA - load accumulator
OPC: $B1
OPR: indirect, Y-indexed
*/
cycles_t i_lda_indr_y(emustate* emu, indr_t opr);

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

/*
LDA - load accumulator
OPC: $B5
OPR: zero-page, X-indexed
*/
cycles_t i_lda_zpg_x(emustate* emu, zpg_t opr);

/*
LDX - load X
OPC: $B6
OPR: zero-page, Y-indexed
*/
cycles_t i_ldx_zpg_y(emustate* emu, zpg_t opr);

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

/*
LDA - load accumulator
OPC: $B9
OPR: absolute, Y-indexed
*/
cycles_t i_lda_abs_y(emustate* emu, abs_t opr);

/*
TSX - transfer stack pointer to X
OPC: $BA
OPR: implied
*/
cycles_t i_tsx(emustate* emu);

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

/*
LDA - load accumulator
OPC: $BD
OPR: absolute, X-indexed
*/
cycles_t i_lda_abs_x(emustate* emu, abs_t opr);

/*
LDX - load X
OPC: $BE
OPR: absolute, Y-indexed
*/
cycles_t i_ldx_abs_y(emustate* emu, abs_t opr);

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

/*
CMP - compare with accumulator
OPC: $C1
OPR: X-indexed, indirect
*/
cycles_t i_cmp_indr_x(emustate* emu, indr_t opr);
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

/*
CMP - compare with accumulator
OPC: $C5
OPR: zero-page
*/
cycles_t i_cmp_zpg(emustate* emu, zpg_t opr);

/*
DEC - decrement
OPC: $C6
OPR: zero-page
*/
cycles_t i_dec_zpg(emustate* emu, zpg_t opr);

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

/*
CMP - compare with accumulator
OPC: $C9
OPR: immediate
*/
cycles_t i_cmp_imd(emustate* emu, imd_t opr);

/*
DEX - decrement X
OPC: $CA
OPR: implied
*/
cycles_t i_dex(emustate* emu);

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

/*
CMP - compare with accumulator
OPC: $CD
OPR: absolute
*/
cycles_t i_cmp_abs(emustate* emu, abs_t opr);

/*
DEC - decrement
OPC: $CE
OPR: absolute
*/
cycles_t i_dec_abs(emustate* emu, abs_t opr);

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

/*
CMP - compare with accumulator
OPC: $D1
OPR: indirect, Y-indexed
*/
cycles_t i_cmp_indr_y(emustate* emu, indr_t opr);

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

/*
DEC - decrement
OPC: $D6
OPR: zero-page, X-indexed
*/
cycles_t i_dec_zpg_x(emustate* emu, zpg_t opr);

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

/*
CMP - compare with accumulator
OPC: $D9
OPR: absolute, Y-indexed
*/
cycles_t i_cmp_abs_y(emustate* emu, abs_t opr);

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

/*
DEC - decrement
OPC: $DE
OPR: absolute, X-indexed
*/
cycles_t i_dec_abs_x(emustate* emu, abs_t opr);

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

/*
SBC - subtract with carry
OPC: $E1
OPR: X-indexed, indirect
*/
cycles_t i_sbc_indr_x(emustate* emu, indr_t opr);

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

/*
SBC - subtract with carry
OPC: $E5
OPR: zero-page
*/
cycles_t i_sbc_zpg(emustate* emu, zpg_t opr);

/*
INC - increment
OPC: $E6
OPR: zero-page
*/
cycles_t i_inc_zpg(emustate* emu, zpg_t opr);

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

/*
SBC - subtract with carry
OPC: $E9
OPR: immediate
*/
cycles_t i_sbc_imd(emustate* emu, imd_t opr);

/*
NOP - no operation
OPC: $EA
OPR: implied
*/
cycles_t i_nop(emustate* emu);

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

/*
SBC - subtract with carry
OPC: $ED
OPR: absolute
*/
cycles_t i_sbc_abs(emustate* emu, abs_t opr);

/*
INC - increment
OPC: $EE
OPR: absolute
*/
cycles_t i_inc_abs(emustate* emu, abs_t opr);

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

/*
SBC - subtract with carry
OPC: $F1
OPR: indirect, Y-indexed
*/
cycles_t i_sbc_indr_y(emustate* emu, indr_t opr);

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

/*
INC - increment
OPC: $F6
OPR: zero-page, X-indexed
*/
cycles_t i_inc_zpg_x(emustate* emu, zpg_t opr);

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

/*
SBC - subtract with carry
OPC: $F9
OPR: absolute, Y-indexed
*/
cycles_t i_sbc_abs_y(emustate* emu, abs_t opr);

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

/*
INC - increment
OPC: $FE
OPR: absolute, X-indexed
*/
cycles_t i_inc_abs_x(emustate* emu, abs_t opr);

/*
RESERVED
OPC: $FF
*/

#endif