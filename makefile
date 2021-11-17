CC=gcc
CFLAGS=-Wall -g3

unit_test: instructions.o unit_test.o addr_idx.o bcd.o
	$(CC) -o $@ $^ $(CFLAGS)

6502emu: 6502emu.o instructions.o addr_idx.o bcd.o instr_map.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o
	rm -f unit_test 6502emu

.PHONY: clean