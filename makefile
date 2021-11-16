CC=gcc
CFLAGS=-Wall -g3

unit_test: instructions.o unit_test.o addr_idx.o
	$(CC) -o $@ $^ $(CFLAGS)


clean:
	rm *.o
	rm unit_test

.PHONY: clean