CC = gcc
CFLAGS = -Wall -Wextra -I..

COMMON = ../logic.c ../gates.c ../flipFlops.c ../print.c ../hacks.c ../ram.c ../alu.c ../ir.c ../cu.c

ram_8x8: ram8x8_demo.c $(COMMON)
	$(CC) $(CFLAGS) ram8x8_demo.c $(COMMON) -o ram_8x8

ram_64k: ram8x8_demo.c $(COMMON)
	$(CC) $(CFLAGS) ram8x8_demo.c $(COMMON) -o ram_64k

counter8: counter8.c $(COMMON)
	$(CC) $(CFLAGS) counter8.c $(COMMON) -o counter8

counter16: counter16.c $(COMMON)
	$(CC) $(CFLAGS) counter16.c $(COMMON) -o counter16

alu: alu_demo.c $(COMMON)
	$(CC) $(CFLAGS) alu_demo.c $(COMMON) -o alu

ir: ir_demo.c $(COMMON)
	$(CC) $(CFLAGS) ir_demo.c $(COMMON) -o ir

cu: cu_demo.c $(COMMON)
	$(CC) $(CFLAGS) cu_demo.c $(COMMON) -o cu

clean:
	rm -f counter8 counter16 ram_8x8 ram_64k alu ir cu