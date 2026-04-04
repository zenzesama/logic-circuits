/**
 * @file alu_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the ALU: ADD, SUB, ADC, SBB, carry chain, zero flag
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "../include/logic.h"

static void printState(alu* a, char* label){
    byte acc = aluGetAccumulator(a);
    char acc_str[5];
    byteToHex(acc, acc_str);
    printf("%-30s | ACC=%s  CARRY=%d  ZERO=%d\n",
        label, acc_str, aluGetCarry(a), aluGetZero(a));
}

int main(void){
    printf("ALU Demonstration\n");
    printf("=================\n\n");

    alu a = newALU();

    /* --- basic ADD --- */
    printf("-- ADD --\n");
    aluLoad(&a, hexToByte("0x56"));
    printState(&a, "LOD 0x56");
    runALU(&a, "0x20", hexToByte("0x2A"));
    printState(&a, "ADD 0x2A  (0x56+0x2A=0x80)");

    /* --- basic SUB --- */
    printf("\n-- SUB --\n");
    aluLoad(&a, hexToByte("0x80"));
    runALU(&a, "0x21", hexToByte("0x38"));
    printState(&a, "SUB 0x38  (0x80-0x38=0x48)");

    /* --- zero flag --- */
    printf("\n-- Zero flag --\n");
    aluLoad(&a, hexToByte("0x42"));
    runALU(&a, "0x21", hexToByte("0x42"));
    printState(&a, "SUB 0x42  (0x42-0x42=0x00)");

    /* --- carry flag on overflow --- */
    printf("\n-- Carry flag --\n");
    aluLoad(&a, hexToByte("0xFF"));
    runALU(&a, "0x20", hexToByte("0x01"));
    printState(&a, "ADD 0x01  (0xFF+0x01 overflow)");

    /* --- 16-bit addition via ADD + ADC ---
     * 76ABh + 232Ch = 99D7h
     * low bytes:  0xAB + 0x2C = 0xD7, no carry
     * high bytes: 0x76 + 0x23 = 0x99, carry in = 0 */
    printf("\n-- 16-bit ADD: 0x76AB + 0x232C = 0x99D7 --\n");
    aluLoad(&a, hexToByte("0xAB"));
    runALU(&a, "0x20", hexToByte("0x2C"));
    printState(&a, "ADD lo: 0xAB+0x2C=0xD7");
    byte lo_result = aluGetAccumulator(&a);
    char lo_str[5]; byteToHex(lo_result, lo_str);

    aluLoad(&a, hexToByte("0x76"));
    runALU(&a, "0x22", hexToByte("0x23"));
    printState(&a, "ADC hi: 0x76+0x23+carry=0x99");
    byte hi_result = aluGetAccumulator(&a);
    char hi_str[5]; byteToHex(hi_result, hi_str);
    printf("16-bit result: %s%s\n", hi_str, lo_str + 2);

    /* --- 16-bit with carry propagation ---
     * 76ABh + 236Ch = 9A17h
     * low bytes:  0xAB + 0x6C = 0x17, carry = 1
     * high bytes: 0x76 + 0x23 + 1 = 0x9A */
    printf("\n-- 16-bit ADD with carry: 0x76AB + 0x236C = 0x9A17 --\n");
    aluLoad(&a, hexToByte("0xAB"));
    runALU(&a, "0x20", hexToByte("0x6C"));
    printState(&a, "ADD lo: 0xAB+0x6C=0x17 carry=1");
    lo_result = aluGetAccumulator(&a);
    byteToHex(lo_result, lo_str);

    aluLoad(&a, hexToByte("0x76"));
    runALU(&a, "0x22", hexToByte("0x23"));
    printState(&a, "ADC hi: 0x76+0x23+1=0x9A");
    hi_result = aluGetAccumulator(&a);
    byteToHex(hi_result, hi_str);
    printf("16-bit result: %s%s\n", hi_str, lo_str + 2);

    return 0;
}