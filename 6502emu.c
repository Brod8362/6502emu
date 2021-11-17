#include "emustate.h"
#include "instructions.h"
#include "instr_map.h"
#include "types.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>

/*
Read 8 bit value in memory stored at current program counter, and increment program counter
*/
uint8_t read_8(emustate* emu) {
    uint8_t v = emu->memory[emu->pc/256][emu->pc%256];
    emu->pc++; //this is done because trying to do it in one line may use undefined behavior
    return v;
}

/*
Read 16 bit value in memory stored at current program counter and PC+1, and increment program counter 
*/
uint16_t read_16(emustate* emu) {
    return read_8(emu) | (read_8(emu) << 8);
}

void reset_proc(emustate* emu) {
    emu->a=0;
    emu->pc=0;
    emu->sp=0xFF;
    emu->sr=(1 << 5); //bit 5 should always be set
    emu->x=0;
    emu->y=0;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            emu->memory[i][j] = 0;
        }
    }   
}

int main(int argc, char** argv) {
    
    emustate emu;
    reset_proc(&emu);
    int clockspeed = 1000000; //1Mhz
    abs_t adr = 0x4000;
    uint8_t byte;
    while (read(STDIN_FILENO, &byte, 1) > 0) { //read program from stdin , starting at mem address 0x4000
        emu.memory[adr/256][adr%256] = byte;
        adr++;
    }
    printf("Read %d program bytes into memory\n", adr-0x4000);

    emu.pc = 0x4000; //set program counter to beginning of program in memory
    while (1) {
        uint8_t opcode = read_8(&emu);
        const instr_info* i = instr_map[opcode];
        if (i == NULL) {
            printf("Invalid opcode $%02x\n @ $%04x\n", opcode, emu.pc-1);
            return 1;
        }
        printf("Decoded instruction %s ($%02x) @ $%04x\n", i->name, opcode, emu.pc-1);
        cycles_t cycles;
        switch (i->type) {
            case Implied:
                cycles = i->fptr.implied(&emu);
                break;
            case Relative:
                cycles = i->fptr.relative(&emu, read_8(&emu));
                break;
            case Zeropage:
                cycles = i->fptr.zpg(&emu, read_8(&emu));
                break;
            case Absolute:
                cycles = i->fptr.absolute(&emu, read_16(&emu));
                break;
            case Immediate:
                cycles = i->fptr.immediate(&emu, read_16(&emu));
                break;
            case Indirect:
                cycles = i->fptr.indirect(&emu, read_16(&emu));
                break;
            default:
                cycles = 0;
                printf("Failed to determine instruction type");
                return 2;
        }
        
        //TODO sleep with cycles
        int sleep_time = ((float)cycles/clockspeed)*1000000; //determine time for cycles to occur, convert to microseconds
        printf("%s ($%02x) took %d cycles to execute\n", i->name, opcode, cycles);
        usleep(sleep_time);
    }

    return 0;
}