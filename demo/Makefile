CC = gcc
CFLAGS = -Wall -Wextra -I..

COMMON = ../src/logic.c ../src/gates.c ../src/flipFlops.c ../src/print.c ../src/hacks.c ../src/ram.c ../src/alu.c ../src/ir.c ../src/cu.c ../src/cpu.c

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

cpu: cpu_demo.c $(COMMON)
	$(CC) $(CFLAGS) cpu_demo.c $(COMMON) -o cpu

all: ram_8x8 ram_64k counter8 counter16 alu ir cu cpu

clean:
	rm -f counter8 counter16 ram_8x8 ram_64k alu ir cu cpu