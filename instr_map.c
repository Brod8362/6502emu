#include "instr_map.h"
#include "instructions.h"
#include "types.h"

static const instr_info s_brk = {"BRK", 0x00, Implied, {.implied = i_brk} };
static const instr_info s_ora_indr_x = {"ORA", 0x01, Indirect, {.indirect = i_ora_indr_x} };
static const instr_info s_ora_zpg = {"ORA", 0x05, Immediate, {.zpg = i_ora_zpg} };
static const instr_info s_asl_zpg = {"ASL", 0x06, Zeropage, {.zpg = i_asl_zpg} };
static const instr_info s_php = {"PHP", 0x08, Implied, {.implied = i_php} };
static const instr_info s_ora_imd = {"ORA", 0x09, Immediate, {.immediate = i_ora_imd} };
static const instr_info s_asl_a = {"ASL", 0x0A, Implied, {.implied = i_asl_a} };
static const instr_info s_ora_abs = {"ORA", 0x0D, Absolute, {.absolute = i_ora_abs} };
static const instr_info s_asl_abs = {"ASL", 0x0E, Absolute, {.absolute = i_asl_abs} };
static const instr_info s_bpl_rel = {"BPL", 0x10, Relative, {.relative = i_bpl_rel} };
static const instr_info s_ora_indr_y = {"ORA", 0x11, Indirect, {.indirect = i_ora_indr_y} };
static const instr_info s_ora_zpg_x = {"ORA", 0x15, Zeropage, {.zpg = i_ora_zpg_x} };
static const instr_info s_asl_zpg_x = {"ASL", 0x16, Zeropage, {.zpg = i_asl_zpg_x} };
static const instr_info s_clc = {"CLC", 0x18, Implied, {.implied = i_clc} };
static const instr_info s_ora_abs_y = {"ORA", 0x19, Absolute, {.absolute = i_ora_abs_y} };
static const instr_info s_ora_abs_x = {"ORA", 0x1D, Absolute, {.absolute = i_ora_abs_x} };
static const instr_info s_asl_abs_x = {"ASL", 0x1E, Absolute, {.absolute = i_asl_abs_x} };
static const instr_info s_jsr_abs = {"JSR", 0x20, Absolute, {.absolute = i_jsr_abs} };
static const instr_info s_and_indr_x = {"AND", 0x21, Indirect, {.indirect = i_and_indr_x} };
static const instr_info s_bit_zpg = {"BIT", 0x24, Zeropage, {.zpg = i_bit_zpg} };
static const instr_info s_and_zpg = {"AND", 0x25, Zeropage, {.zpg = i_and_zpg} };
static const instr_info s_rol_zpg = {"ROL", 0x26, Zeropage, {.zpg = i_rol_zpg} };
static const instr_info s_plp = {"PLP", 0x28, Implied, {.implied = i_plp} };
static const instr_info s_and_imd = {"AND", 0x29, Immediate, {i_and_imd} };
static const instr_info s_rol_a = {"ROL", 0x2A, Implied, {.implied = i_rol_a} };
static const instr_info s_bit_abs = {"BIT", 0x2C, Absolute, {.absolute = i_bit_abs} };
static const instr_info s_and_abs = {"AND", 0x2D, Absolute, {.absolute = i_and_abs} };
static const instr_info s_rol_abs = {"ROL", 0x2E, Absolute, {.absolute = i_rol_abs} };
static const instr_info s_bmi_rel = {"BMI", 0x30, Relative, {.relative = i_bmi_rel} };
static const instr_info s_and_indr_y = {"AND", 0x31, Indirect, {.indirect = i_and_indr_y} };
static const instr_info s_and_zpg_x = {"AND", 0x35, Zeropage, {.zpg = i_and_zpg_x} };
static const instr_info s_rol_zpg_x = {"ROL", 0x36, Zeropage, {.zpg = i_rol_zpg_x} };
static const instr_info s_sec = {"SEC", 0x38, Implied, {.implied = i_sec} };
static const instr_info s_and_abs_y = {"AND", 0x39, Absolute, {.absolute = i_and_abs_y} };
static const instr_info s_and_abs_x = {"AND", 0x3D, Absolute, {.absolute = i_and_abs_x} };
static const instr_info s_rol_abs_x = {"ROL", 0x3E, Absolute, {.absolute = i_rol_abs_x} };
static const instr_info s_rti = {"RTI", 0x40, Implied, {.implied = i_rti} };
static const instr_info s_eor_indr_x = {"EOR", 0x41, Indirect, {.indirect = i_eor_indr_x} };
static const instr_info s_eor_zpg = {"EOR", 0x45, Zeropage, {.zpg = i_eor_zpg} };
static const instr_info s_lsr_zpg = {"LSR", 0x46, Zeropage, {.zpg = i_lsr_zpg} };
static const instr_info s_pha = {"PHA", 0x48, Implied, {.implied = i_pha} };
static const instr_info s_eor_imd = {"EOR", 0x49, Immediate, {i_eor_imd} };
static const instr_info s_lsr_a = {"LSR", 0x4A, Implied, {.implied = i_lsr_a} };
static const instr_info s_jmp_abs = {"JMP", 0x4C, Absolute, {.absolute = i_jmp_abs} };
static const instr_info s_eor_abs = {"EOR", 0x4D, Absolute, {.absolute = i_eor_abs} };
static const instr_info s_lsr_abs = {"LSR", 0x4E, Absolute, {.absolute = i_lsr_abs} };
static const instr_info s_bvc_rel = {"BVC", 0x50, Relative, {.relative = i_bvc_rel} };
static const instr_info s_eor_indr_y = {"EOR", 0x51, Indirect, {.indirect = i_eor_indr_y} };
static const instr_info s_eor_zpg_x = {"EOR", 0x55, Zeropage, {.zpg = i_eor_zpg_x} };
static const instr_info s_lsr_zpg_x = {"LSR", 0x56, Zeropage, {.zpg = i_lsr_zpg_x} };
static const instr_info s_cli = {"CLI", 0x58, Implied, {.implied = i_cli} };
static const instr_info s_eor_abs_y = {"EOR", 0x59, Absolute, {.absolute = i_eor_abs_y} };
static const instr_info s_eor_abs_x = {"EOR", 0x5D, Absolute, {.absolute = i_eor_abs_x} };
static const instr_info s_lsr_abs_x = {"LSR", 0x5E, Absolute, {.absolute = i_lsr_abs_x} };
static const instr_info s_rts = {"RTS", 0x60, Implied, {.implied = i_rts} };
static const instr_info s_adc_indr_x = {"ADC", 0x61, Indirect, {.indirect = i_adc_indr_x} };
static const instr_info s_adc_zpg = {"ADC", 0x65, Zeropage, {.zpg = i_adc_zpg} };
static const instr_info s_ror_zpg = {"ROR", 0x66, Zeropage, {.zpg = i_ror_zpg} };
static const instr_info s_pla = {"PLA", 0x68, Implied, {.implied = i_pla} };
static const instr_info s_adc_imd = {"ADC", 0x69, Immediate, {i_adc_imd} };
static const instr_info s_ror_a = {"ROR", 0x6A, Implied, {.implied = i_ror_a} };
static const instr_info s_jmp_indr = {"JMP", 0x6C, Indirect, {.indirect = i_jmp_indr} };
static const instr_info s_adc_abs = {"ADC", 0x6D, Absolute, {.absolute = i_adc_abs} };
static const instr_info s_ror_abs = {"ROR", 0x6E, Absolute, {.absolute = i_ror_abs} };
static const instr_info s_bvs_rel = {"BVS", 0x70, Relative, {.relative = i_bvs_rel} };
static const instr_info s_adc_indr_y = {"ADC", 0x71, Indirect, {.indirect = i_adc_indr_y} };
static const instr_info s_adc_zpg_x = {"ADC", 0x75, Zeropage, {.zpg = i_adc_zpg_x} };
static const instr_info s_ror_zpg_x = {"ROR", 0x76, Zeropage, {.zpg = i_ror_zpg_x} };
static const instr_info s_sei = {"SEI", 0x78, Implied, {.implied = i_sei} };
static const instr_info s_adc_abs_y = {"ADC", 0x79, Absolute, {.absolute = i_adc_abs_y} };
static const instr_info s_adc_abs_x = {"ADC", 0x7D, Absolute, {.absolute = i_adc_abs_x} };
static const instr_info s_ror_abs_x = {"ROR", 0x7E, Absolute, {.absolute = i_ror_abs_x} };
static const instr_info s_sta_indr_x = {"STA", 0x81, Indirect, {.indirect = i_sta_indr_x} };
static const instr_info s_sty_zpg = {"STY", 0x84, Zeropage, {.zpg = i_sty_zpg} };
static const instr_info s_sta_zpg = {"STA", 0x85, Zeropage, {.zpg = i_sta_zpg} };
static const instr_info s_stx_zpg = {"STX", 0x86, Zeropage, {.zpg = i_stx_zpg} };
static const instr_info s_dey = {"DEY", 0x88, Implied, {.implied = i_dey} };
static const instr_info s_txa = {"TXA", 0x8A, Implied, {.implied = i_txa} };
static const instr_info s_sty_abs = {"STY", 0x8C, Absolute, {.absolute = i_sty_abs} };
static const instr_info s_sta_abs = {"STA", 0x8D, Absolute, {.absolute = i_sta_abs} };
static const instr_info s_stx_abs = {"STX", 0x8E, Absolute, {.absolute = i_stx_abs} };
static const instr_info s_bcc_rel = {"BCC", 0x90, Relative, {.relative = i_bcc_rel} };
static const instr_info s_sta_indr_y = {"STA", 0x91, Indirect, {.indirect = i_sta_indr_y} };
static const instr_info s_sty_zpg_x = {"STY", 0x94, Zeropage, {.zpg = i_sty_zpg_x} };
static const instr_info s_sta_zpg_x = {"STA", 0x95, Zeropage, {.zpg = i_sta_zpg_x} };
static const instr_info s_stx_zpg_y = {"STX", 0x96, Zeropage, {.zpg = i_stx_zpg_y} };
static const instr_info s_tya = {"TYA", 0x98, Implied, {.implied = i_tya} };
static const instr_info s_sta_abs_y = {"STA", 0x99, Absolute, {.absolute = i_sta_abs_y} };
static const instr_info s_txs = {"TXA", 0x9A, Implied, {.implied = i_txs} };
static const instr_info s_sta_abs_x = {"STA", 0x9D, Absolute, {.absolute = i_sta_abs_x} };
static const instr_info s_ldy_imd = {"LDY", 0xA0, Immediate, {i_ldy_imd} };
static const instr_info s_lda_indr_x = {"LDA", 0xA1, Indirect, {.indirect = i_lda_indr_x} };
static const instr_info s_ldx_imd = {"LDX", 0xA2, Immediate, {i_ldx_imd} };
static const instr_info s_ldy_zpg = {"LDY", 0xA4, Zeropage, {.zpg = i_ldy_zpg} };
static const instr_info s_lda_zpg = {"LDA", 0xA5, Zeropage, {.zpg = i_lda_zpg} };
static const instr_info s_ldx_zpg = {"LDX", 0xA6, Zeropage, {.zpg = i_ldx_zpg} };
static const instr_info s_tay = {"TAY", 0xA8, Implied, {.implied = i_tay} };
static const instr_info s_lda_imd = {"LDA", 0xA9, Immediate, {i_lda_imd} };
static const instr_info s_tax = {"TAX", 0xAA, Implied, {.implied = i_tax} };
static const instr_info s_ldy_abs = {"LDY", 0xAC, Absolute, {.absolute = i_ldy_abs} };
static const instr_info s_lda_abs = {"LDA", 0xAD, Absolute, {.absolute = i_lda_abs} };
static const instr_info s_ldx_abs = {"LDX", 0xAE, Absolute, {.absolute = i_ldx_abs} };
static const instr_info s_bcs_rel = {"BCS", 0xB0, Relative, {.relative = i_bcs_rel} };
static const instr_info s_lda_indr_y = {"LDA", 0xB1, Indirect, {.indirect = i_lda_indr_y} };
static const instr_info s_ldy_zpg_x = {"LDY", 0xB4, Zeropage, {.zpg = i_ldy_zpg_x} };
static const instr_info s_lda_zpg_x = {"LDA", 0xB5, Zeropage, {.zpg = i_lda_zpg_x} };
static const instr_info s_ldx_zpg_y = {"LDX", 0xB6, Zeropage, {.zpg = i_ldx_zpg_y} };
static const instr_info s_clv = {"CLV", 0xB8, Implied, {.implied = i_clv} };
static const instr_info s_lda_abs_y = {"LDA", 0xB9, Absolute, {.absolute = i_lda_abs_y} };
static const instr_info s_tsx = {"TSX", 0xBA, Implied, {.implied = i_tsx} };
static const instr_info s_ldy_abs_x = {"LDY", 0xBC, Absolute, {.absolute = i_ldy_abs_x} };
static const instr_info s_lda_abs_x = {"LDA", 0xBD, Absolute, {.absolute = i_lda_abs_x} };
static const instr_info s_ldx_abs_y = {"LDX", 0xBE, Absolute, {.absolute = i_ldx_abs_y} };
static const instr_info s_cpy_imd = {"CPY", 0xC0, Immediate, {i_cpy_imd} };
static const instr_info s_cmp_indr_x = {"CMP", 0xC1, Indirect, {.indirect = i_cmp_indr_x} };
static const instr_info s_cpy_zpg = {"CPY", 0xC4, Zeropage, {.zpg = i_cpy_zpg} };
static const instr_info s_cmp_zpg = {"CMP", 0xC5, Zeropage, {.zpg = i_cmp_zpg} };
static const instr_info s_dec_zpg = {"DEC", 0xC6, Zeropage, {.zpg = i_dec_zpg} };
static const instr_info s_iny = {"INY", 0xC8, Implied, {.implied = i_iny} };
static const instr_info s_cmp_imd = {"CMP", 0xC9, Immediate, {i_cmp_imd} };
static const instr_info s_dex = {"DEX", 0xCA, Implied, {.implied = i_dex} };
static const instr_info s_cpy_abs = {"CPY", 0xCC, Absolute, {.absolute = i_cpy_abs} };
static const instr_info s_cmp_abs = {"CMP", 0xCD, Absolute, {.absolute = i_cmp_abs} };
static const instr_info s_dec_abs = {"DEC", 0xCE, Absolute, {.absolute = i_dec_abs} };
static const instr_info s_bne_rel = {"BNE", 0xD0, Relative, {.relative = i_bne_rel} };
static const instr_info s_cmp_indr_y = {"CMP", 0xD1, Indirect, {.indirect = i_cmp_indr_y} };
static const instr_info s_cmp_zpg_x = {"CMP", 0xD5, Zeropage, {.zpg = i_cmp_zpg_x} };
static const instr_info s_dec_zpg_x = {"DEC", 0xD6, Zeropage, {.zpg = i_dec_zpg_x} };
static const instr_info s_cld = {"CLD", 0xD8, Implied, {.implied = i_cld} };
static const instr_info s_cmp_abs_y = {"CMP", 0xD9, Absolute, {.absolute = i_cmp_abs_y} };
static const instr_info s_cmp_abs_x = {"CMP", 0xDD, Absolute, {.absolute = i_cmp_abs_x} };
static const instr_info s_dec_abs_x = {"DEC", 0xDE, Absolute, {.absolute = i_dec_abs_x} };
static const instr_info s_cpx_imd = {"CPX", 0xE0, Immediate, {i_cpx_imd} };
static const instr_info s_sbc_indr_x = {"SBC", 0xE1, Indirect, {.indirect = i_sbc_indr_x} };
static const instr_info s_cpx_zpg = {"CPX", 0xE4, Zeropage, {.zpg = i_cpx_zpg} };
static const instr_info s_sbc_zpg = {"SBC", 0xE5, Zeropage, {.zpg = i_sbc_zpg} };
static const instr_info s_inc_zpg = {"INC", 0xE6, Zeropage, {.zpg = i_inc_zpg} };
static const instr_info s_inx = {"INX", 0xE8, Implied, {.implied = i_inx} };
static const instr_info s_sbc_imd = {"SBC", 0xE9, Immediate, {i_sbc_imd} };
static const instr_info s_nop = {"NOP", 0xEA, Implied, {.implied = i_nop} };
static const instr_info s_cpx_abs = {"CPX", 0xEC, Absolute, {.absolute = i_cpx_abs} };
static const instr_info s_sbc_abs = {"SBC", 0xED, Absolute, {.absolute = i_sbc_abs} };
static const instr_info s_inc_abs = {"INC", 0xEE, Absolute, {.absolute = i_inc_abs} };
static const instr_info s_beq_rel = {"BEQ", 0xF0, Relative, {.relative = i_beq_rel} };
static const instr_info s_sbc_indr_y = {"SBC", 0xF1, Indirect, {.indirect = i_sbc_indr_y} };
static const instr_info s_sbc_zpg_x = {"SBC", 0xF5, Zeropage,  {.zpg = i_sbc_zpg_x} };
static const instr_info s_inc_zpg_x = {"INC", 0xF6, Zeropage, {.zpg = i_inc_zpg_x} };
static const instr_info s_sed = {"SED", 0xF8, Implied, {.implied = i_sed} };
static const instr_info s_sbc_abs_y = {"SBC", 0xF9, Absolute, {.absolute = i_sbc_abs_y} };
static const instr_info s_sbc_abs_x = {"SBC", 0xFD, Absolute, {.absolute = i_sbc_abs_x} };
static const instr_info s_inc_abs_x = {"INC", 0xFE, Absolute, {.absolute = i_inc_abs_x} };

const instr_info* instr_map[256] = {
    &s_brk, //$00
    &s_ora_indr_x, //$01
    NULL, //$02
    NULL, //$03
    NULL, //$04
    &s_ora_zpg, //$05
    &s_asl_zpg, //$06
    NULL, //$07
    &s_php, //$08
    &s_ora_imd, //$09
    &s_asl_a, //$0A
    NULL, //$0B
    NULL, //$0C
    &s_ora_abs, //$0D
    &s_asl_abs, //$0E
    NULL, //$0F
    &s_bpl_rel, //$10
    &s_ora_indr_y, //$11
    NULL, //$12
    NULL, //$13
    NULL, //$14
    &s_ora_zpg_x, //$15
    &s_asl_zpg_x, //$16
    NULL, //$17
    &s_clc, //$18
    &s_ora_abs_y, //$19
    NULL, //$1A
    NULL, //$1B
    NULL, //$1C
    &s_ora_abs_x, //$1D
    &s_asl_abs_x, //$1E
    NULL, //$1F
    &s_jsr_abs, //$20
    &s_and_indr_x, //$21
    NULL, //$22
    NULL, //$23
    &s_bit_zpg, //$24
    &s_and_zpg,//$25
    &s_rol_zpg, //$26
    NULL, //$27
    &s_plp, //$28
    &s_and_imd, //$29
    &s_rol_a, //$2A
    NULL, //$2B
    &s_bit_abs, //$2C
    &s_and_abs, //$2D
    &s_rol_abs, //$2E
    NULL, //$2F
    &s_bmi_rel, //$30
    &s_and_indr_y, //$31
    NULL, //$32
    NULL, //$33
    NULL, //$34
    &s_and_zpg_x, //$35
    &s_rol_zpg_x, //$36
    NULL, //$37
    &s_sec, //$38
    &s_and_abs_y, //$39
    NULL, //$3A
    NULL, //$3B
    NULL, //$3C
    &s_and_abs_x, //$3D
    &s_rol_abs_x, //$3E
    NULL, //$3F
    &s_rti, //$40
    &s_eor_indr_x,//$41
    NULL, //$42
    NULL, //$43
    NULL, //$44
    &s_eor_zpg, //$45
    &s_lsr_zpg, //$46
    NULL, //$47
    &s_pha,//$48
    &s_eor_imd, //$49
    &s_lsr_a, //$4A
    NULL, //$4B
    &s_jmp_abs, //$4C
    &s_eor_abs, //$4D
    &s_lsr_abs, //$4E
    NULL, //$4F
    &s_bvc_rel, //$50
    &s_eor_indr_y, //$51
    NULL, //$52
    NULL, //$53
    NULL, //$54
    &s_eor_zpg_x, //$55
    &s_lsr_zpg_x, //$56
    NULL, //$57
    &s_cli, //$58
    &s_eor_abs_y, //$59
    NULL, //$5A
    NULL, //$5B
    NULL, //$5C
    &s_eor_abs_x, //$5D
    &s_lsr_abs_x, //$5E
    NULL, //$5F
    &s_rts, //$60
    &s_adc_indr_x, //$61
    NULL, //$62
    NULL, //$63
    NULL, //$64
    &s_adc_zpg, //$65
    &s_ror_zpg, //$66
    NULL, //$67
    &s_pla, //$68
    &s_adc_imd,//$69
    &s_ror_a, //$6A
    NULL, //$6B
    &s_jmp_indr, //$6C
    &s_adc_abs, //$6D
    &s_ror_abs, //$6E
    NULL, //$6F
    &s_bvs_rel, //$70
    &s_adc_indr_y, //$71
    NULL, //$72
    NULL, //$73
    NULL, //$74
    &s_adc_zpg_x, //$75
    &s_ror_zpg_x, //$76
    NULL, //$77
    &s_sei, //$78
    &s_adc_abs_y, //$79
    NULL, //$7A
    NULL, //$7B
    NULL, //$7C
    &s_adc_abs_x, //$7D
    &s_ror_abs_x, //$7E
    NULL, //$7F
    NULL, //$80
    &s_sta_indr_x, //$81
    NULL, //$82
    NULL, //$83
    &s_sty_zpg, //$84
    &s_sta_zpg, //$85
    &s_stx_zpg, //$86
    NULL, //$87
    &s_dey, //$88
    NULL, //$89
    &s_txa, //$8A
    NULL, //$8B
    &s_sty_abs, //$8C
    &s_sta_abs, //$8D
    &s_stx_abs, //$8E
    NULL, //$8F
    &s_bcc_rel, //$90
    &s_sta_indr_y, //$91
    NULL, //$92
    NULL, //$93
    &s_sty_zpg_x, //$94
    &s_sta_zpg_x, //$95
    &s_stx_zpg_y, //$96
    NULL, //$97
    &s_tya, //$98
    &s_sta_abs_y, //$99
    &s_txs, //$9A
    NULL, //$9B
    NULL, //$9C
    &s_sta_abs_x, //$9D
    NULL, //$9E
    NULL, //$9F
    &s_ldy_imd, //$A0
    &s_lda_indr_x, //$A1
    &s_ldx_imd, //$A2
    NULL, //$A3
    &s_ldy_zpg, //$A4
    &s_lda_zpg, //$A5
    &s_ldx_zpg, //$A6
    NULL, //$A7
    &s_tay, //$A8
    &s_lda_imd, //$A9
    &s_tax, //$AA
    NULL, //$AB
    &s_ldy_abs, //$AC
    &s_lda_abs, //$AD
    &s_ldx_abs, //$AE
    NULL, //$AF
    &s_bcs_rel, //$B0
    &s_lda_indr_y, //$B1
    NULL, //$B2
    NULL, //$B3
    &s_ldy_zpg_x, //$B4
    &s_cmp_zpg_x, //$B5
    &s_ldx_zpg_y, //$B6
    NULL, //$B7
    &s_clv, //$B8
    &s_lda_abs_y, //$B9
    &s_tsx, //$BA
    NULL, //$BB
    &s_ldy_abs_x, //$BC
    &s_lda_abs_x, //$BD
    &s_ldx_abs_y, //$BE
    NULL, //$BF
    &s_cpy_imd, //$C0
    &s_cmp_indr_x, //$C1
    NULL, //$C2
    NULL, //$C3
    &s_cpy_zpg, //$C4
    &s_cmp_zpg, //$C5
    &s_dec_zpg, //$C6
    NULL, //$C7
    &s_iny, //$C8
    &s_cmp_imd, //$C9
    &s_dex, //$CA
    NULL, //$CB
    &s_cpy_abs, //$CC
    &s_cmp_abs, //$CD
    &s_dec_abs, //$CE
    NULL, //$CF
    &s_bne_rel, //$D0
    &s_cmp_indr_y, //$D1
    NULL, //$D2
    NULL, //$D3
    NULL, //$D4
    &s_lda_zpg_x, //$D5
    &s_dec_zpg_x, //$D6
    NULL, //$D7
    &s_cld, //$D8
    &s_cmp_abs_y, //$D9
    NULL, //$DA
    NULL, //$DB
    NULL, //$DC
    &s_cmp_abs_x, //$DD
    &s_dec_abs_x, //$DE
    NULL, //$DF
    &s_cpx_imd, //$E0
    &s_sbc_indr_x, //$E1
    NULL, //$E2
    NULL, //$E3
    &s_cpx_zpg, //$E4
    &s_sbc_zpg, //$E5
    &s_inc_zpg, //$E6
    NULL, //$E7
    &s_inx, //$E8
    &s_sbc_imd, //$E9
    &s_nop, //$EA
    NULL, //$EB
    &s_cpx_abs, //$EC
    &s_sbc_abs, //$ED
    &s_inc_abs, //$EE
    NULL, //$EF
    &s_beq_rel, //$F0
    &s_sbc_indr_y, //$F1
    NULL, //$F2
    NULL, //$F3
    NULL, //$F4
    &s_sbc_zpg_x, //$F5
    &s_inc_zpg_x, //$F6
    NULL, //$F7
    &s_sed, //$F8
    &s_sbc_abs_y, //$F9
    NULL, //$FA
    NULL, //$FB
    NULL, //$FC
    &s_sbc_abs_x,//$FD
    &s_inc_abs_x, //$FE
    NULL //#FF
};
