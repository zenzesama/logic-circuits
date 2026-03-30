/**
 * @file cu.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Control Unit - sequences the fetch-execute cycle and drives all
 *        other CPU components in response to the current instruction
 * @version 0.1
 * @date 2026-03-29
 * 
 * @copyright Copyright (c) 2026
 * 
 * 
 * The control unit references to every other component:
 *   - sixteen_bit_counter (program counter)
 *   - ram64kx8 (unified code + data memory)
 *   - instruction_register (holds the current 3-byte instruction)
 *   - alu (accumulator + carry + zero flag)
 *
 * Each call to stepCU() performs one complete fetch-execute cycle:
 *
 *   Fetch (3 sub-cycles, one per instruction byte):
 *     1. read RAM[PC] → IR opcode,   PC++
 *     2. read RAM[PC] → IR addr_hi,  PC++
 *     3. read RAM[PC] → IR addr_lo,  PC++
 *
 *   Execute (1 sub-cycle):
 *     decode opcode → drive ALU / RAM / PC accordingly
 *
 * Supported opcodes:
 *   0x10  LOD — load RAM[addr] into accumulator
 *   0x11  STO — store accumulator into RAM[addr]
 *   0x20  ADD — accumulator + RAM[addr]
 *   0x21  SUB — accumulator - RAM[addr]
 *   0x22  ADC — accumulator + RAM[addr] + carry
 *   0x23  SBB — accumulator - RAM[addr] - carry
 *   0x30  JMP — unconditional jump to addr
 *   0x31  JZ  — jump if zero flag == 1
 *   0x32  JC  — jump if carry flag == 1
 *   0x33  JNZ — jump if zero flag == 0
 *   0x34  JNC — jump if carry flag == 0
 *   0xFF  HLT — set halted flag, return CU_HALTED
 */

#include "logic.h"
#include <stdlib.h>

/**
 * @brief reads the current program counter value as a 16-bit integer
 *
 * @param cu pointer to the control unit
 * @return int the current PC value in range 0x0000-0xFFFF
 */
static int pcToInt(control_unit* cu) {
    byte hi, lo;
    getSixteenBitCounterValue(cu->pc, &hi, &lo);
    return (byteToDecimal(hi) << 8) | byteToDecimal(lo);
}

/**
 * @brief reads one byte from RAM at an arbitrary 16-bit integer address
 *
 * @param cu pointer to the control unit
 * @param addr the 16-bit address as an integer
 * @return byte the byte read from RAM
 *
 * used for both instruction fetch (pass pcToInt) and data reads.
 */
static byte ramRead(control_unit* cu, int addr) {
    byte hi_byte = hexToByte("0x00");
    byte lo_byte = hexToByte("0x00");
 
    int hi = (addr >> 8) & 0xFF;
    int lo =  addr & 0xFF;
    for(int i = 0; i < 8; i++) {
        hi_byte.bits[7-i] = (hi >> i) & 1;
        lo_byte.bits[7-i] = (lo >> i) & 1;
    }
 
    char hi_str[5], lo_str[5];
    byteToHex(hi_byte, hi_str);
    byteToHex(lo_byte, lo_str);
    return readRam64kx8(cu->ram, hi_str, lo_str);
}

/**
 * @brief writes one byte to RAM at an arbitrary 16-bit integer address
 *
 * @param cu pointer to the control unit
 * @param addr the 16-bit address as an integer
 * @param data the byte to write
 */
static void ramWrite(control_unit* cu, int addr, byte data) {
    byte hi_byte = hexToByte("0x00");
    byte lo_byte = hexToByte("0x00");
 
    int hi = (addr >> 8) & 0xFF;
    int lo =  addr & 0xFF;
    for(int i = 0; i < 8; i++) {
        hi_byte.bits[7-i] = (hi >> i) & 1;
        lo_byte.bits[7-i] = (lo >> i) & 1;
    }
 
    char hi_str[5], lo_str[5];
    byteToHex(hi_byte, hi_str);
    byteToHex(lo_byte, lo_str);
    writeRam64kx8(cu->ram, hi_str, lo_str, data);
}

/**
 * @brief loads the jump address from the IR into the program counter
 *
 * @param cu pointer to the control unit
 */
static void doJump(control_unit* cu) {
    byte hi = irGetAddrHi(cu->ir);
    byte lo = irGetAddrLo(cu->ir);
    char hi_str[5], lo_str[5];
    byteToHex(hi, hi_str);
    byteToHex(lo, lo_str);
    jumpSixteenBitCounter(cu->pc, hi_str, lo_str);
}

/**
 * @brief initializes a new control unit, allocating all components on the heap
 *
 * @return control_unit* pointer to the initialized control unit
 *
 * the RAM is heap-allocated (64KB). all other components are embedded in
 * the struct. the caller owns this pointer - pass it to freeCU() when done.
 */
control_unit* newCU(void) {
    control_unit* cu = (control_unit*)malloc(sizeof(control_unit));
 
    cu->pc = (sixteen_bit_counter*)malloc(sizeof(sixteen_bit_counter));
    cu->ir = (instruction_register*)malloc(sizeof(instruction_register));
    cu->a  = (alu*)malloc(sizeof(alu));
 
    *cu->pc = newSixteenBitCounter();
    *cu->ir = newInstructionRegister();
    *cu->a = newALU();
    cu->ram = newRam64kx8();
 
    cu->halted = 0;
    return cu;
}

/**
 * @brief frees all heap memory owned by the control unit
 *
 * @param cu pointer to the control unit to free
 */
void freeCU(control_unit* cu){
    free(cu->ram);
    free(cu->pc);
    free(cu->ir);
    free(cu->a);
    free(cu);
}

/**
 * @brief writes an array of bytes into RAM starting at address 0x0000
 *
 * @param cu pointer to the control unit
 * @param program array of byte values (as ints 0-255) to load
 * @param length number of bytes to load
 *
 * this is the equivalent of toggling the switches on the control panel
 * to enter a program. bytes are written sequentially from 0x0000.
 */
void loadProgram(control_unit* cu, int* program, int length) {
    for(int i = 0; i < length; i++) {
        byte b = hexToByte("0x00");
        int val = program[i];
        for(int bit = 0; bit < 8; bit++) {
            b.bits[7-bit] = (val >> bit) & 1;
        }
        ramWrite(cu, i, b);
    }
}

/**
 * @brief advances the control unit by one full fetch-execute cycle
 *
 * @param cu pointer to the control unit
 * @return cu_status CU_RUNNING if execution should continue, CU_HALTED if
 *                   a HLT instruction was executed or the unit was already
 *                   halted before this call
 *
 * fetch phase (3 bytes, one per clock sub-cycle):
 *   reads the opcode and 2 address bytes from RAM into the IR,
 *   incrementing the PC after each byte.
 *
 * execute phase (1 clock sub-cycle):
 *   decodes the opcode from the IR and drives the appropriate component.
 *
 * conditional jumps check the ALU flags after the fetch but before the PC
 * has moved on - if the condition is not met, execution continues normally.
 */
cu_status stepCU(control_unit* cu) {
    if(cu->halted) return CU_HALTED;
 
    /* --- fetch --- */
    byte op_byte = ramRead(cu, pcToInt(cu)); tickSixteenBitCounter(cu->pc);
    byte hi_byte = ramRead(cu, pcToInt(cu)); tickSixteenBitCounter(cu->pc);
    byte lo_byte = ramRead(cu, pcToInt(cu)); tickSixteenBitCounter(cu->pc);
 
    /* load into IR */
    char op_str[5], hi_str[5], lo_str[5];
    byteToHex(op_byte, op_str);
    byteToHex(hi_byte, hi_str);
    byteToHex(lo_byte, lo_str);
    loadIR(cu->ir, op_str, hi_str, lo_str);
 
    /* --- execute --- */
    int opcode = byteToDecimal(op_byte);
    int addr = irGetAddress(cu->ir);
 
    switch(opcode) {
         case 0x10:  /* LOD */
            aluLoad(cu->a, ramRead(cu, addr));
            break;
 
        case 0x11:  /* STO */
            ramWrite(cu, addr, aluGetAccumulator(cu->a));
            break;
 
        case 0x20: { /* ADD */
            byte operand = ramRead(cu, addr);
            runALU(cu->a, op_str, operand);
            break;
        }
        case 0x21: { /* SUB */
            byte operand = ramRead(cu, addr);
            runALU(cu->a, op_str, operand);
            break;
        }
        case 0x22: { /* ADC */
            byte operand = ramRead(cu, addr);
            runALU(cu->a, op_str, operand);
            break;
        }
        case 0x23: { /* SBB */
            byte operand = ramRead(cu, addr);
            runALU(cu->a, op_str, operand);
            break;
        }
 
        case 0x30:  /* JMP */
            doJump(cu);
            break;
 
        case 0x31:  /* JZ */
            if(aluGetZero(cu->a)  == HIGH) doJump(cu);
            break;
 
        case 0x32:  /* JC */
            if(aluGetCarry(cu->a) == HIGH) doJump(cu);
            break;
 
        case 0x33:  /* JNZ */
            if(aluGetZero(cu->a)  == LOW)  doJump(cu);
            break;
 
        case 0x34:  /* JNC */
            if(aluGetCarry(cu->a) == LOW)  doJump(cu);
            break;
 
        case 0xFF:  /* HLT */
            cu->halted = 1;
            return CU_HALTED;
 
        default:
            break;
    }
 
    return CU_RUNNING;
}