/**
 * @file ir.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-28
 * 
 * @copyright Copyright (c) 2026
 * 
 * Each instruction is exactly 3 bytes:
 *
 *   Byte 0 - opcode (e.g. 0x20 = ADD)
 *   Byte 1 - addr_hi (high byte of the 16-bit data address)
 *   Byte 2 - addr_lo (low  byte of the 16-bit data address)
 *
 * The exception is HLT (0xFF) which has no address, but we still
 * allocate all 3 latches for uniformity — the CPU simply ignores
 * addr_hi and addr_lo when the opcode is HLT.
 *
 * All three latches are clocked simultaneously during the fetch cycle
 * once the PC has stepped through the 3 bytes in RAM.
 */

#include "../include/logic.h"


/**
 * @brief initializes a new instruction register with all latches cleared
 *
 * @return instruction_register the initialized IR
 */
instruction_register newInstructionRegister(void) {
    instruction_register ir;
    ir.opcode = newEightBitLatch();
    ir.addr_hi = newEightBitLatch();
    ir.addr_lo = newEightBitLatch();
    return ir;
}

/**
 * @brief loads a full 3-byte instruction into the register
 *
 * @param ir pointer to the instruction register
 * @param opcode the instruction opcode byte, e.g. "0x20"
 * @param addr_hi high byte of the 16-bit address, e.g. "0x40"
 * @param addr_lo low  byte of the 16-bit address, e.g. "0x00"
 *
 * all three latches are clocked HIGH then LOW to store the new values,
 * mirroring how the control panel clocks the latches during a fetch cycle.
 */
void loadIR(instruction_register* ir, char* opcode, char* addr_hi, char* addr_lo) {
    byte op = hexToByte(opcode);
    byte hi = hexToByte(addr_hi);
    byte lo = hexToByte(addr_lo);
 
    eightBitLatch(&ir->opcode, op, HIGH);
    eightBitLatch(&ir->opcode, op, LOW);
 
    eightBitLatch(&ir->addr_hi, hi, HIGH);
    eightBitLatch(&ir->addr_hi, hi, LOW);
 
    eightBitLatch(&ir->addr_lo, lo, HIGH);
    eightBitLatch(&ir->addr_lo, lo, LOW);
}

/**
 * @brief reads the opcode byte currently held in the register
 *
 * @param ir pointer to the instruction register
 * @return byte the opcode byte
 */
byte irGetOpcode(instruction_register* ir){
    return eightBitLatchOutput(&ir->opcode);
}

/**
 * @brief reads the high byte of the address currently held in the register
 *
 * @param ir pointer to the instruction register
 * @return byte the high address byte
 */
byte irGetAddrHi(instruction_register* ir){
    return eightBitLatchOutput(&ir->addr_hi);
}

/**
 * @brief reads the low byte of the address currently held in the register
 *
 * @param ir pointer to the instruction register
 * @return byte the low address byte
 */
byte irGetAddrLo(instruction_register* ir){
    return eightBitLatchOutput(&ir->addr_lo);
}

/**
 * @brief returns the full 16-bit data address as a single integer
 *
 * @param ir pointer to the instruction register
 * @return int the address in range 0x0000–0xFFFF
 *
 * convenience helper so the control unit does not need to manually
 * combine hi and lo bytes when computing the RAM address.
 */
int irGetAddress(instruction_register* ir){
    int hi = byteToDecimal(eightBitLatchOutput(&ir->addr_hi));
    int lo = byteToDecimal(eightBitLatchOutput(&ir->addr_lo));
    return (hi << 8) | lo;
}