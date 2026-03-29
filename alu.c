/**
 * @file alu.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Arithmetic Logic Unit
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 *
 * Supported opcodes:
 *   0x20  ADD — accumulator + operand, carry in = 0
 *   0x21  SUB — accumulator - operand, borrow in = 0
 *   0x22  ADC — accumulator + operand + carry latch
 *   0x23  SBB — accumulator - operand - borrow latch
 *
 * After every operation:
 *   - result is stored back into the accumulator
 *   - carry/borrow out is latched into the carry latch
 *   - zero flag is set to 1 if the result is 0x00, else 0
 *
 * LOD (0x10) and STO (0x11) are handled by the CPU, not the ALU.
 * The carry and zero latches are NOT updated by LOD/STO.
 */

#include "logic.h"

/**
 * @brief returns HIGH if all 8 bits of b are LOW, else LOW
 *
 * @param b the byte to test
 * @return bit HIGH if b == 0x00, LOW otherwise
 *
 * chained NOR: NOR(b0,b1), NOR(b2,b3), NOR(b4,b5), NOR(b6,b7)
 * then NOR those four results pairwise down to one bit.
 * a NOR of two bits is HIGH only if both are LOW, so chaining them
 * produces HIGH only when every input bit is LOW.
 */
static bit zeroDetector(byte b) {
    bit n01 = norGate(b.bits[0], b.bits[1]);
    bit n23 = norGate(b.bits[2], b.bits[3]);
    bit n45 = norGate(b.bits[4], b.bits[5]);
    bit n67 = norGate(b.bits[6], b.bits[7]); // 67 🥀
 
    bit n0123 = norGate(notGate(n01), notGate(n23));
    bit n4567 = norGate(notGate(n45), notGate(n67));
 
    return norGate(notGate(n0123), notGate(n4567));
}

/**
 * @brief initializes a new ALU with accumulator, carry latch, and zero latch
 *        all cleared to zero
 *
 * @return alu the initialized ALU
 */
alu newALU(void) {
    alu a;
    a.accumulator = newEightBitLatch();
    a.carry_latch = newDFlipFlop();
    a.zero_latch = newDFlipFlop();
 
    byte zero = hexToByte("0x00");
    eightBitLatch(&a.accumulator, zero, HIGH);
    eightBitLatch(&a.accumulator, zero, LOW);
 
    return a;
}

/**
 * @brief loads a value directly into the accumulator (LOD instruction)
 *
 * @param a pointer to the ALU
 * @param data the byte to load
 *
 * does NOT update the carry or zero latches
 */
void aluLoad(alu* a, byte data) {
    eightBitLatch(&a->accumulator, data, HIGH);
    eightBitLatch(&a->accumulator, data, LOW);
}

/**
 * @brief reads the current accumulator value
 *
 * @param a pointer to the ALU
 * @return byte the value currently in the accumulator
 */
byte aluGetAccumulator(alu* a) {
    return eightBitLatchOutput(&a->accumulator);
}

/**
 * @brief reads the current carry flag
 *
 * @param a pointer to the ALU
 * @return bit HIGH if carry/borrow was set by the last arithmetic op
 */
bit aluGetCarry(alu* a) {
    return a->carry_latch.rs_ff.q;
}
 
/**
 * @brief reads the current zero flag
 *
 * @param a pointer to the ALU
 * @return bit HIGH if the last arithmetic result was 0x00
 */
bit aluGetZero(alu* a) {
    return a->zero_latch.rs_ff.q;
}

/**
 * @brief executes one ALU operation using the given opcode
 *
 * @param a pointer to the ALU
 * @param opcode hex opcode string: "0x20" ADD, "0x21" SUB,
 *                                  "0x22" ADC, "0x23" SBB
 * @param operand the byte value read from RAM (the second operand)
 *
 * the accumulator is always the first operand.
 * after the operation:
 *   - result is written back into the accumulator
 *   - carry latch is clocked with the carry/borrow out bit
 *   - zero latch is clocked with the output of the zero detector
 *
 * an unrecognised opcode leaves all state unchanged.
 *
 * opcode truth table:
 *   0x20  ADD:  acc + operand, carry_in = LOW
 *   0x21  SUB:  acc - operand, borrow_in = LOW
 *   0x22  ADC:  acc + operand + carry latch
 *   0x23  SBB:  acc - operand - carry latch
 */
void runALU(alu* a, char* opcode, byte operand) {
    byte acc = eightBitLatchOutput(&a->accumulator);
    bit c = a->carry_latch.rs_ff.q;
 
    byte result;
    bit carry_out;
    int valid = 1;
 
    int code = byteToDecimal(hexToByte(opcode));
 
    switch(code){
        case 0x20:  /* ADD */
            result = eightBitAdder(acc, operand, LOW, &carry_out);
            break;
        case 0x21:  /* SUB */
            result = eightBitSubtractor(acc, operand, LOW, &carry_out);
            break;
        case 0x22:  /* ADC */
            result = eightBitAdder(acc, operand, c, &carry_out);
            break;
        case 0x23:  /* SBB */
            result = eightBitSubtractor(acc, operand, c, &carry_out);
            break;
        default:
            valid = 0;
            break;
    }
 
    if(!valid) return;
 
    eightBitLatch(&a->accumulator, result, HIGH);
    eightBitLatch(&a->accumulator, result, LOW);
 
    dFlipFlop(&a->carry_latch, carry_out, HIGH);
    dFlipFlop(&a->carry_latch, carry_out, LOW);
 
    bit zero_flag = zeroDetector(result);
    dFlipFlop(&a->zero_latch, zero_flag, HIGH);
    dFlipFlop(&a->zero_latch, zero_flag, LOW);
}