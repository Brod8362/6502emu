CC=gcc
CFLAGS=-Wall -g3 -Isrc

bin/instr_test: src/instructions.o test/instr_test.o src/addr_idx.o src/bcd.o
	mkdir -p bin
	$(CC) -o $@ $^ $(CFLAGS)

bin/6502emu: src/6502emu.o src/instructions.o src/addr_idx.o src/bcd.o src/instr_map.o
	mkdir -p bin
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o *.o65 src/*.o test/*.o
	rm -rf bin

.PHONY: clean