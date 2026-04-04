/**
 * @file cpu.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief public api for control unit, registers and step debugging.
 * @version 0.1
 * @date 2026-03-31
 * 
 * @copyright Copyright (c) 2026
 * 
 * cpu struct has its own control unit and extends it with two things:
 * - cycle counter: total instructions executed since last reset
 * - debug mode: when enables, it prints state after each instruction using stepCPU
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "../include/logic.h"

/**
 * @brief prints the current state of cpu to stdout
 * 
 * @param c pointer to the cpu
 * 
 * prints cycle count, last ran op code, program counter, carry bit and zero bit.
 * All in one line.
 */
static void printCPUState(cpu* c) {
    byte acc = cpuGetAccumulator(c);
    byte pc_hi, pc_lo, op_code;
    char acc_srt[5], hi_str[5], lo_str[5], op_str[5];

    getSixteenBitCounterValue(c->cu->pc, &pc_hi, &pc_lo);
    op_code = irGetOpcode(c->cu->ir);

    byteToHex(acc, acc_srt);
    byteToHex(pc_hi, hi_str);
    byteToHex(pc_lo, lo_str);
    byteToHex(op_code, op_str);

    printf("[cycle %4d] opcode = %s  PC = %s%s  ACC = %s  CARRY = %d ZERO = %d\n", 
            c->cycles, op_str, hi_str, lo_str + 2, acc_srt, cpuGetCarry(c), cpuGetZero(c));
}

/**
 * @brief allocates and initializes a new cpu
 * 
 * @return cpu* pointer to the new initialized cpu
 * 
 * allocates a control unit, which itself allocates ram and all sub-components, 
 * cycle count starting at 0 and debug mode turned off.
 * Pass to freeCPU() when done.
 */
cpu* newCPU(void) {
    cpu* c = (cpu*)malloc(sizeof(cpu));
    c->cu = newCU();
    c->cycles = 0;
    c->debug = 0;
    return c;
}

/**
 * @brief frees the memory used by cpu
 * 
 * @param c pointer to the cpu to free
 */
void freeCPU(cpu* c) {
    freeCU(c->cu);
    free(c);
}

/**
 * @brief reset the cpu to initial state without re allocating
 * 
 * @param c pointer to the cpu
 * 
 * clears the program counter, ALU (accumulator, carry and zero flag) and cycle counter.
 * Ram contents are preserved.
 */
void resetCPU(cpu* c) {
    resetSixteenBitCounter(c->cu->pc);
    *c->cu->a = newALU();
    *c->cu->ir = newInstructionRegister();
    c->cu->halted = 0;
    c->cycles = 0;
}

/**
 * @brief enables or disables debug
 * 
 * @param c pointer to the cpu
 * @param debug 1 to enable debug printing and 0 to disable
 * 
 * when debug is enables, stepCPU prints state each instruction.
 */
void cpuSetDebug(cpu* c, int debug) {
    c->debug = debug;
}

/**
 * @brief loads a program into ram starting at 0x0000
 * 
 * @param c pointer to the CPU
 * @param program array of values
 * @param length number of values of array
 */
void cpuLoad(cpu* c, int* program, int length) {
    loadProgram(c->cu, program, length);
}

/**
 * @brief reads the current accumulator value
 * 
 * @param c pointer to the cpu
 * @return byte the accumulator value
 */
byte cpuGetAccumulator(cpu* c) {
    return aluGetAccumulator(c->cu->a);
}

/**
 * @brief reads the current program counter value as a 16 bit integer.
 * 
 * @param c pointer to the cpu
 * @return int pc value in range 0x0000 to 0xFFFF
 */
int cpuGetPC(cpu* c) {
    byte hi, lo;
    getSixteenBitCounterValue(c->cu->pc, &hi, &lo);

    return (byteToDecimal(hi) << 8) | byteToDecimal(lo);
}

/**
 * @brief reads the current carry flag
 *
 * @param c pointer to the CPU
 * @return bit HIGH if carry was set by the last arithmetic operation
 */
bit cpuGetCarry(cpu* c) {
    return aluGetCarry(c->cu->a);
}
 
/**
 * @brief reads the current zero flag
 *
 * @param c pointer to the CPU
 * @return bit HIGH if the last arithmetic result was 0x0000
 */
bit cpuGetZero(cpu* c) {
    return aluGetZero(c->cu->a);
}

/**
 * @brief reads 1 byte directly from ram at a given address
 * 
 * @param c pointer to the cpu
 * @param addr 16-bit address as integer
 * @return byte the byte stored in that address
 * 
 * useful for reading result after the program is done running.
 */
byte cpuReadRAM(cpu* c, int addr) {
    byte hi_byte = hexToByte("0x00");
    byte lo_byte = hexToByte("0x00");

    int hi, lo;
    hi = (addr >> 8) & 0xFF;
    lo = addr & 0xFF;

    for (int i = 0; i < 8; i++) {
        hi_byte.bits[7 - i] = (hi >> i) & 1;
        lo_byte.bits[7 - i] = (lo >> i) & 1;
    }

    char hi_str[5], lo_str[5];

    byteToHex(hi_byte, hi_str);
    byteToHex(lo_byte, lo_str);

    return readRam64kx8(c->cu->ram, hi_str, lo_str);
}

/**
 * @brief returns the total number of instructions executed since last reset
 *
 * @param c pointer to the CPU
 * @return int the cycle count
 */
int cpuGetCycles(cpu* c) {
    return c->cycles;
}

/**
 * @brief executes one instruction and advanced cpu by one cycle
 * 
 * @param c pointer to the cpu
 * @return cu_status running or halted (CU_RUNNING or CU_HALTED)
 * 
 * increments the cycle counter after each instruction.
 * if debug mode is on, prints CPU status after execution
 */
cu_status stepCPU(cpu* c) {
    cu_status status = stepCU(c->cu);
    c->cycles++;
    if (c->debug) printCPUState(c);
    return status;
}

/**
 * @brief runs the CPU until HLT instruction is reached
 * 
 * @param c pointer to the cpu
 * @return int the total number of instructions ran
 * 
 * calls stepCPU in a loop until CU_HALTED is returned.
 * state is printed each instruction if debug mode is on.
 */
int runCPU(cpu* c) {
    while (stepCPU(c) == CU_RUNNING);
    return c->cycles;
}