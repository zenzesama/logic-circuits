/**
 * @file cu_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the control unit running real programs from the book
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "logic.h"

static void printState(control_unit* cu){
    byte acc = aluGetAccumulator(cu->a);
    char acc_str[5];
    byteToHex(acc, acc_str);

    byte hi, lo;
    char hi_str[5], lo_str[5];
    getSixteenBitCounterValue(cu->pc, &hi, &lo);
    byteToHex(hi, hi_str);
    byteToHex(lo, lo_str);

    byte ir_op = irGetOpcode(cu->ir);
    char op_str[5]; byteToHex(ir_op, op_str);

    printf("  opcode=%s  PC=%s%s  ACC=%s  CARRY=%d  ZERO=%d\n",
        op_str, hi_str, lo_str + 2,
        acc_str, aluGetCarry(cu->a), aluGetZero(cu->a));
}

static void runProgram(control_unit* cu){
    cu_status status;
    do {
        status = stepCU(cu);
        printState(cu);
    } while(status == CU_RUNNING);
}

int main(void){
    printf("Control Unit Demonstration\n");
    printf("==========================\n\n");

    /* ---------------------------------------------------------------
     * Program 1: 0x56 + 0x2A - 0x38 = 0x48  (book p.216)
     *
     *   0x0000: 10 00 10  LOD [0x0010]
     *   0x0003: 20 00 11  ADD [0x0011]
     *   0x0006: 21 00 12  SUB [0x0012]
     *   0x0009: 11 00 13  STO [0x0013]
     *   0x000C: FF 00 00  HLT
     *   0x000F: 00        padding
     *   0x0010: 56 2A 38  data
     * --------------------------------------------------------------- */
    printf("Program 1: 0x56 + 0x2A - 0x38 (expected 0x48)\n");
    printf("-----------------------------------------------\n");

    control_unit* cu = newCU();
    int prog1[] = {
        0x10, 0x00, 0x10,
        0x20, 0x00, 0x11,
        0x21, 0x00, 0x12,
        0x11, 0x00, 0x13,
        0xFF, 0x00, 0x00,
        0x00,
        0x56, 0x2A, 0x38
    };
    loadProgram(cu, prog1, sizeof(prog1)/sizeof(prog1[0]));
    runProgram(cu);

    byte r1 = readRam64kx8(cu->ram, "0x00", "0x13");
    char r1_str[5]; byteToHex(r1, r1_str);
    printf("  => result at 0x0013 = %s\n", r1_str);
    freeCU(cu);

    /* ---------------------------------------------------------------
     * Program 2: 16-bit  0x76AB + 0x236C = 0x9A17
     *
     *   0x0000: 10 00 20  LOD [0x0020]   load lo byte 0xAB
     *   0x0003: 20 00 22  ADD [0x0022]   add  lo byte 0x6C -> 0x17 carry=1
     *   0x0006: 11 00 24  STO [0x0024]   store lo result
     *   0x0009: 10 00 21  LOD [0x0021]   load hi byte 0x76
     *   0x000C: 22 00 23  ADC [0x0023]   add  hi byte 0x23 + carry -> 0x9A
     *   0x000F: 11 00 25  STO [0x0025]   store hi result
     *   0x0012: FF 00 00  HLT
     *   (pad to 0x0020)
     *   0x0020: AB 76 6C 23  data
     * --------------------------------------------------------------- */
    printf("\nProgram 2: 16-bit 0x76AB + 0x236C (expected 0x9A17)\n");
    printf("------------------------------------------------------\n");

    cu = newCU();
    int prog2[] = {
        0x10, 0x00, 0x20,  // LOD [0x0020]   load lo byte 0xAB
        0x20, 0x00, 0x22,  // ADD [0x0022]   add lo byte 0x6C
        0x11, 0x00, 0x24,  // STO [0x0024]   store lo result
        0x10, 0x00, 0x21,  // LOD [0x0021]   load hi byte 0x76
        0x22, 0x00, 0x23,  // ADC [0x0023]   add hi byte 0x23 + carry
        0x11, 0x00, 0x25,  // STO [0x0025]   store hi result
        0xFF, 0x00, 0x00,  // HLT
        0xAB, 0x76, 0x6C, 0x23  // Data bytes
    };
    loadProgram(cu, prog2, sizeof(prog2)/sizeof(prog2[0]));
    runProgram(cu);

    byte lo_r = readRam64kx8(cu->ram, "0x00", "0x24");
    byte hi_r = readRam64kx8(cu->ram, "0x00", "0x25");
    char lo_s[5], hi_s[5];
    byteToHex(lo_r, lo_s); byteToHex(hi_r, hi_s);
    printf("  => result = %s%s\n", hi_s, lo_s + 2);
    freeCU(cu);

    return 0;
}