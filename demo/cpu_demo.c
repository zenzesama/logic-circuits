/**
 * @file cpu_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the CPU: arithmetic, 16-bit addition, and multiplication
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "../include/logic.h"

int main(void){
    printf("CPU Demonstration\n");
    printf("=================\n\n");

    /* ---------------------------------------------------------------
     * Program 1: 0x56 + 0x2A - 0x38 = 0x48
     * debug mode on to show each instruction
     * --------------------------------------------------------------- */
    printf("Program 1: 0x56 + 0x2A - 0x38 (debug on)\n");
    printf("-----------------------------------------\n");

    cpu* c = newCPU();
    cpuSetDebug(c, 1);

    int prog1[] = {
        0x10, 0x00, 0x10,   /* LOD [0x0010] */
        0x20, 0x00, 0x11,   /* ADD [0x0011] */
        0x21, 0x00, 0x12,   /* SUB [0x0012] */
        0x11, 0x00, 0x13,   /* STO [0x0013] */
        0xFF, 0x00, 0x00,   /* HLT          */
        0x00,               /* padding      */
        0x56, 0x2A, 0x38    /* data         */
    };
    cpuLoad(c, prog1, sizeof(prog1)/sizeof(prog1[0]));
    int cycles = runCPU(c);

    byte r = cpuReadRAM(c, 0x0013);
    char r_str[5]; byteToHex(r, r_str);
    printf("  result=%-6s  cycles=%d\n", r_str, cycles);
    freeCPU(c);

    /* ---------------------------------------------------------------
     * Program 2: 16-bit  0x76AB + 0x236C = 0x9A17
     * debug mode off — just show the result and cycle count
     * --------------------------------------------------------------- */
    printf("\nProgram 2: 16-bit 0x76AB + 0x236C (debug off)\n");
    printf("----------------------------------------------\n");

    c = newCPU();

    int prog2[] = {
        0x10, 0x00, 0x20,   /* LOD [0x0020]  load lo 0xAB      */
        0x20, 0x00, 0x22,   /* ADD [0x0022]  add  lo 0x6C      */
        0x11, 0x00, 0x24,   /* STO [0x0024]  store lo result   */
        0x10, 0x00, 0x21,   /* LOD [0x0021]  load hi 0x76      */
        0x22, 0x00, 0x23,   /* ADC [0x0023]  add  hi 0x23+carry*/
        0x11, 0x00, 0x25,   /* STO [0x0025]  store hi result   */
        0xFF, 0x00, 0x00,   /* HLT                              */
        /* pad to 0x0020 */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xAB, 0x76, 0x6C, 0x23
    };
    cpuLoad(c, prog2, sizeof(prog2)/sizeof(prog2[0]));
    cycles = runCPU(c);

    byte lo_r = cpuReadRAM(c, 0x0024);
    byte hi_r = cpuReadRAM(c, 0x0025);
    char lo_s[5], hi_s[5];
    byteToHex(lo_r, lo_s); byteToHex(hi_r, hi_s);
    printf("  result=0x%s%s  cycles=%d\n", hi_s + 2, lo_s + 2, cycles);
    freeCPU(c);

    /* ---------------------------------------------------------------
     * Program 3: multiply 0xA7 * 0x1C = 0x1234 (16-bit result)
     * from the book p.228 — repeated addition via JNZ loop
     *
     * memory layout:
     *   0x1000: 00 A7  multiplier  (0x00A7)
     *   0x1002: 00 1C  multiplicand (0x001C = 28 decimal)
     *   0x1004: 00 00  product lo (result)
     *   0x1006: 00 00  product hi (result)
     *
     * algorithm: add multiplier to product 28 times (once per
     * unit of multiplicand), decrementing multiplicand each loop.
     * loop exits when multiplicand reaches zero (JNZ -> JZ exits).
     *
     * 0x0000: LOD [0x1005]        load product lo
     * 0x0003: ADD [0x1001]        add multiplier lo
     * 0x0006: STO [0x1005]        store product lo
     * 0x0009: LOD [0x1004]        load product hi
     * 0x000C: ADC [0x1000]        add multiplier hi + carry
     * 0x000F: STO [0x1004]        store product hi
     * 0x0012: LOD [0x1003]        load multiplicand lo
     * 0x0015: SUB [0x0021]        subtract 1 (decrement)
     * 0x0018: STO [0x1003]        store decremented value
     * 0x001B: JNZ 0x0000          loop if not zero
     * 0x001E: FF 00 00  HLT
     * 0x0021: 01         constant 1
     * --------------------------------------------------------------- */
    printf("\nProgram 3: 0xA7 * 0x1C = ? (multiplication loop)\n");
    printf("-------------------------------------------------\n");

    c = newCPU();
    cpuSetDebug(c, 1);

    int prog3[] = {
        /* 0x0000 */ 0x10, 0x10, 0x05,   /* LOD [0x1005] product lo      */
        /* 0x0003 */ 0x20, 0x10, 0x01,   /* ADD [0x1001] multiplier lo   */
        /* 0x0006 */ 0x11, 0x10, 0x05,   /* STO [0x1005]                 */
        /* 0x0009 */ 0x10, 0x10, 0x04,   /* LOD [0x1004] product hi      */
        /* 0x000C */ 0x22, 0x10, 0x00,   /* ADC [0x1000] multiplier hi   */
        /* 0x000F */ 0x11, 0x10, 0x04,   /* STO [0x1004]                 */
        /* 0x0012 */ 0x10, 0x10, 0x03,   /* LOD [0x1003] multiplicand lo */
        /* 0x0015 */ 0x21, 0x00, 0x21,   /* SUB [0x0021] subtract 1      */
        /* 0x0018 */ 0x11, 0x10, 0x03,   /* STO [0x1003]                 */
        /* 0x001B */ 0x33, 0x00, 0x00,   /* JNZ 0x0000                   */
        /* 0x001E */ 0xFF, 0x00, 0x00,   /* HLT                          */
        /* 0x0021 */ 0x01,               /* constant 1 (used by SUB)     */
    };

    cpuLoad(c, prog3, sizeof(prog3)/sizeof(prog3[0]));

    /* write data directly into RAM at 0x1000 */
    int data[] = {
        0x00, 0xA7,   /* 0x1000: multiplier   = 0x00A7 */
        0x00, 0x1C,   /* 0x1002: multiplicand = 0x001C */
        0x00, 0x00,   /* 0x1004: product hi           */
        0x00, 0x00    /* 0x1006: product lo           */
    };
    /* load data at offset 0x1000 */
    for(int i = 0; i < 8; i++){
        byte b = hexToByte("0x00");
        int val = data[i];
        for(int bit = 0; bit < 8; bit++)
            b.bits[7-bit] = (val >> bit) & 1;
        byte hi_byte = hexToByte("0x00");
        byte lo_byte = hexToByte("0x00");
        int addr = 0x1000 + i;
        int hi = (addr >> 8) & 0xFF;
        int lo = addr & 0xFF;
        for(int j = 0; j < 8; j++){
            hi_byte.bits[7-j] = (hi >> j) & 1;
            lo_byte.bits[7-j] = (lo >> j) & 1;
        }
        char hi_str[5], lo_str[5];
        byteToHex(hi_byte, hi_str);
        byteToHex(lo_byte, lo_str);
        writeRam64kx8(c->cu->ram, hi_str, lo_str, b);
    }

    cycles = runCPU(c);

    byte prod_hi = cpuReadRAM(c, 0x1004);
    byte prod_lo = cpuReadRAM(c, 0x1005);
    char ph[5], pl[5];
    byteToHex(prod_hi, ph); byteToHex(prod_lo, pl);
    printf("  0xA7 * 0x1C = 0x%s%s  (%d decimal)  cycles=%d\n",
        ph + 2, pl + 2,
        (byteToDecimal(prod_hi) << 8) | byteToDecimal(prod_lo),
        cycles);
    freeCPU(c);

    return 0;
}