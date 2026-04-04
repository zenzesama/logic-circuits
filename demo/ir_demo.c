/**
 * @file ir_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the instruction register
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "../include/logic.h"


static void printIR(instruction_register* ir) {
    byte op = irGetOpcode(ir);
    byte hi = irGetAddrHi(ir);
    byte lo = irGetAddrLo(ir);
    char op_str[5], hi_str[5], lo_str[5];
    byteToHex(op, op_str);
    byteToHex(hi, hi_str);
    byteToHex(lo, lo_str);
    printf("  opcode=%s  addr=%s%s\n",
        op_str, hi_str, lo_str + 2);
}

int main(void) {
    printf("Instruction Register Demonstration\n");
    printf("===================================\n\n");

    instruction_register ir = newInstructionRegister();

    /* LOD A,[0x4001] - load from address 0x4001 */
    printf("Fetching: LOD  0x4001\n");
    loadIR(&ir, "0x10", "0x40", "0x01");
    printIR(&ir);

    /* ADD A,[0x4003] - add value at 0x4003 */
    printf("\nFetching: ADD  0x4003\n");
    loadIR(&ir, "0x20", "0x40", "0x03");
    printIR(&ir);

    /* ADC A,[0x4002] - add with carry from 0x4002 */
    printf("\nFetching: ADC  0x4002\n");
    loadIR(&ir, "0x22", "0x40", "0x02");
    printIR(&ir);

    /* STO [0x4005],A - store to 0x4005 */
    printf("\nFetching: STO  0x4005\n");
    loadIR(&ir, "0x11", "0x40", "0x05");
    printIR(&ir);

    /* JMP 0x0000 - jump to start */
    printf("\nFetching: JMP  0x0000\n");
    loadIR(&ir, "0x30", "0x00", "0x00");
    printIR(&ir);

    /* HLT - no address used */
    printf("\nFetching: HLT\n");
    loadIR(&ir, "0xFF", "0x00", "0x00");
    printIR(&ir);

    return 0;
}

// ts is so hollow, makes sense too. The IR doesn't have any work besides being a container for others to use. Its crucial but unimportant independently, I'll let this demo be here because why not.