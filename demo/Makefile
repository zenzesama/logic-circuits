CC = gcc
CFLAGS = -Wall -Wextra -I..

COMMON = ../logic.c ../gates.c ../flipFlops.c ../print.c ../hacks.c ../ram.c

ram_8x8: ram8x8_demo.c $(COMMON)
	$(CC) $(CFLAGS) ram8x8_demo.c $(COMMON) -o ram_8x8

ram_64k: ram8x8_demo.c $(COMMON)
	$(CC) $(CFLAGS) ram8x8_demo.c $(COMMON) -o ram_64k

counter: counter.c $(COMMON)
	$(CC) $(CFLAGS) counter.c $(COMMON) -o counter

clean:
	rm -f counter ram_8x8 ram_64k