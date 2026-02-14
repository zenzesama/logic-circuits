/**
 * @file logic.c
 * @author zenze (zenzesama@gmail.com)
 * @brief this file contains all the logic for each component
 * @version 0.1
 * @date 2026-02-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"

/**
 * @brief this is a half adder that taked two bits and returns the takes in 2 bits
 * 
 * @param a this is the first bit
 * @param b this is the second bit
 * @return half_adder this is the new state of the half adder
 * 
 * we first initialize a helf adder, then perform the calculations to get the carry and the sum.
 * then in the half_adder structure, we store these carry and sum and return that half adder.
 */
half_adder halfAdder(bit a, bit b){
    half_adder result;
    result.carry_out = andGate(a, b);
    result.sum_out = xorGate(a, b);
    return result;
}

/**
 * @brief this is a full ader made by using two half adders
 * 
 * @param a this is the first bit
 * @param b this is the second bit
 * @param carry_in this is carry (if any is needed)
 * @return full_adder this is the returned full_adder structure in new state
 * 
 * we first initialize two half adders h1 and h2, then perform the necessary calculations.
 * Read diagram to unsderstand how this happens.
 * This is similar to the half adder one, where we return the adder with new state.
 */
full_adder fullAdder(bit a, bit b, bit carry_in){
    full_adder result;
    half_adder h1 = halfAdder(a, b);
    half_adder h2 = halfAdder(h1.sum_out, carry_in);

    result.sum_out = h2.sum_out;
    result.carry_out = orGate(h1.carry_out, h2.carry_out);

    return result;
}

/**
 * @brief this is a bull 8-bit adder with carry out functionality.
 * 
 * @param byte_a this is the first byte
 * @param byte_b this is the second byte
 * @param carry this is the carry bit
 * @param carry_out this is the carry out bit pointer
 * @return byte this is the returned byte
 * 
 * we first initialize the result byte.
 * Then we iterate over the byte in reverse order and perform a full adder on each bit with the carry
 * we initialize the result one by one and update the carry each iteration
 * we update the carry out pointer to have the last carry, this can be used in overflow detection
 */
byte eightBitAdder(byte byte_a, byte byte_b, bit carry, bit* carry_out){
    byte sum;

    for(int i = 7; i >= 0; i--){
        full_adder result = fullAdder(byte_a.bits[i], byte_b.bits[i], carry);
        sum.bits[i] = result.sum_out;
        carry = result.carry_out;
    }

    *carry_out = carry;
    return sum;
}

/**
 * @brief this just inverts a byte
 * 
 * @param byte_a this is the input byte
 * @return byte this is the returned byte
 * 
 * here we iterate over each bit in the byte and invert it
 * hence inverting the entire byte
 */
byte notByte(byte byte_a){
    byte result;
    for(int i = 0; i < 8; i++){
        result.bits[i] = notGate(byte_a.bits[i]);
    }
    return result;
}

/**
 * @brief this is an 8-bit subtractor built using two's complement arithmetic
 * 
 * @param byte_a this is the minuend (the number being subtracted from)
 * @param byte_b this is the subtrahend (the number being subtracted)
 * @param borrow_in this is the initial borrow bit
 * @param borrow_out this is a pointer to the resulting borrow bit
 * @return byte this is the result of byte_a - byte_b
 * 
 * this subtractor works by using two's complement.
 * first, we invert byte_b.
 * then we add it to byte_a along with the inverted borrow_in.
 * subtraction is implemented as:
 *      A - B = A + (~B) + 1
 * 
 * the final carry_out from the adder is inverted to produce borrow_out.
 * this allows to detecch underflow
 */
byte eightBitSubtractor(byte byte_a, byte byte_b, bit borrow_in, bit* borrow_out){
    byte b_invert = notByte(byte_b);
    bit carry_out;

    bit carry_in = notGate(borrow_in);

    byte result = eightBitAdder(byte_a, b_invert, carry_in, &carry_out);

    *borrow_out = notGate(carry_out);
    return result;
}

/**
 * @brief initializes and returns a new 8-bit latch
 * 
 * @return eight_bit_latch this is the initialized latch structure
 * 
 * this function creates an eight_bit_latch and initializes
 * each internal D flip-flop using newDFlipFlop().
 * 
 * each flip-flop is responsible for storing one bit.
 * the latch is returned in its initialized state.
 */
eight_bit_latch newEightBitLatch(void){
    eight_bit_latch latch;
    for(int i = 0; i < 8; i++){
        latch.flip_flops[i] = newDFlipFlop();
    }
    return latch;
}

/**
 * @brief this writes data into an 8-bit latch using a clock signal
 * 
 * @param latch pointer to the latch structure
 * @param data this is the 8-bit input data
 * @param clock this is the clock signal
 * 
 * this function iterates through all 8 bits and feeds each bit
 * into its corresponding D flip-flop.
 * 
 * when the clock is triggered, each flip-flop updates its stored value.
 * this effectively stores the entire byte into the latch.
 */
void eightBitLatch(eight_bit_latch* latch, byte data, bit clock){
    for(int i = 0; i < 8; i++){
        dFlipFlop(&latch->flip_flops[i], data.bits[i], clock);
    }
}

/**
 * @brief reads the stored value from an 8-bit latch
 * 
 * @param latch pointer to the latch structure
 * @return byte this is the byte currently stored in the latch
 * 
 * this function reads the q output of each internal flip-flop
 * and constructs a byte from those values.
 * 
 * the returned byte represents the current stored state of the latch.
 */
byte eightBitLatchOutput(eight_bit_latch* latch){
    byte output;
    for(int i = 0; i < 8; i++){
        output.bits[i] = latch->flip_flops[i].rs_ff.q;
    }
    return output;
}

/**
 * @brief this is a 2-to-1 selector (multiplexer)
 * 
 * @param a first input bit
 * @param b second input bit
 * @param select control bit
 * @return bit selected output
 * 
 * if select is 0, the function returns a.
 * if select is 1, the function returns b.
 * 
 * this is implemented using logic gates:
 *      output = (a AND ~select) OR (b AND select)
 */
bit twoToOneSelector(bit a, bit b, bit select){
    bit notSelect = notGate(select);
    bit a_path = andGate(a, notSelect);
    bit b_path = andGate(b, select);
    return orGate(a_path, b_path);
}

/**
 * @brief this is an 8-bit 2-to-1 selector (multiplexer)
 * 
 * @param a first input byte
 * @param b second input byte
 * @param select control bit
 * @return byte selected output byte
 * 
 * this function applies the twoToOneSelector to each bit
 * of the input bytes.
 * 
 * if select is 0, byte a is returned.
 * if select is 1, byte b is returned.
 * 
 * selection is performed independently for all 8 bits.
 * but has same effect as selecting one of the bytes.
 */
byte eightBitTwoToOneSelector(byte a, byte b, bit select){
    byte output;
    for(int i = 0; i < 8; i++){
        output.bits[i] = twoToOneSelector(a.bits[i], b.bits[i], select);
    }
    return output;
}