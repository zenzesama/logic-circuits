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

#include "../include/logic.h"

/**
 * @brief this is a half adder that takes in two bits and returns the half adder with modified state
 * 
 * @param a this is the first bit
 * @param b this is the second bit
 * @return half_adder this is the new state of the half adder
 * 
 * we first initialize a half adder, then perform the calculations to get the carry and the sum.
 * then in the half_adder structure, we store these carry and sum and return that half adder.
 */
half_adder halfAdder(bit a, bit b){
    half_adder result;
    result.carry_out = andGate(a, b);
    result.sum_out = xorGate(a, b);
    return result;
}

/**
 * @brief this is a full adder made by using two half adders
 * 
 * @param a this is the first bit
 * @param b this is the second bit
 * @param carry_in this is carry (if any is needed)
 * @return full_adder this is the returned full_adder structure in new state
 * 
 * we first initialize two half adders h1 and h2, then perform the necessary calculations.
 * Read diagram to understand how this happens.
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
 * this allows to underflow
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

/**
 * @brief initializes a new adding machine
 * 
 * @return adding_machine the initialized machine
 * 
 * this creates a new adding machine with an empty latch
 * and initializes the last_sum to zero
 */
adding_machine newAddingMachine(void){
    adding_machine calc;
    calc.latch = newEightBitLatch();

    byte zero = {{0,0,0,0,0,0,0,0}};
    calc.last_sum = zero;

    return calc;
}


/**
 * @brief runs the adding machine for one operation
 * 
 * @param calc pointer to the adding machine
 * @param input the byte value to add (from switches)
 * @param from_latch selector bit: LOW = add to zero, HIGH = add to latch value
 * @param save clock bit: HIGH = save result to latch, LOW = just compute
 * 
 * this function:
 * 1. reads the current latch value
 * 2. selects between zero or latch value based on from_latch
 * 3. adds input to the selected value
 * 4. stores the sum
 * 5. optionally saves to latch if save is HIGH
 */
void runAddingMachine(adding_machine* calc, byte input, bit from_latch, bit save){
    byte latch_output = eightBitLatchOutput(&calc->latch);
    
    byte zero = {{0,0,0,0,0,0,0,0}};
    byte selected = eightBitTwoToOneSelector(zero, latch_output, from_latch);

    bit carry_out;
    byte sum = eightBitAdder(input, selected, LOW, &carry_out);

    calc->last_sum = sum;

    eightBitLatch(&calc->latch, sum, save);
}

/**
 * @brief gets the most recent computed sum from the adding machine
 * 
 * @param calc pointer to the adding machine
 * @return byte the last computed sum
 * 
 * this returns the result of the most recent addition operation
 */
byte getAddingMachineSum(adding_machine* calc){
    return calc->last_sum;
}

/**
 * @brief initializes a new 8-bit counter starting at 0
 * 
 * @return eight_bit_counter the initialized counter
 */
eight_bit_counter newEightBitCounter(void){
    eight_bit_counter counter;
    for(int i = 0; i < 8; i++){
        counter.flip_flops[i] = newEdgeTriggeredDFlipFlop();
        counter.flip_flops[i].slave.rs_ff.q = LOW;
        counter.flip_flops[i].slave.rs_ff.q_bar = HIGH;
        counter.flip_flops[i].master.rs_ff.q = LOW;
        counter.flip_flops[i].master.rs_ff.q_bar = HIGH;
    }
    return counter;
}

/**
 * @brief increments the counter by one (simulates one clock tick)
 * 
 * @param counter pointer to the counter to increment
 * 
 * this is a ripple counter where each flip-flop acts as a frequency divider.
 * FF0 toggles every tick. each subsequent FF toggles on falling edges of the
 * previous FF's output (HIGH to LOW transition). this creates an up counter.
 */
void tickEightBitCounter(eight_bit_counter* counter){
    bit saved_q[8];
    for(int i = 0; i < 8; i++){
        saved_q[i] = counter->flip_flops[i].slave.rs_ff.q;
    }
    
    // toggle FF0 (connected to external clock)
    bit q_bar_0 = counter->flip_flops[0].slave.rs_ff.q_bar;
    edgeTriggeredDFlipFlop(&counter->flip_flops[0], q_bar_0, LOW);
    edgeTriggeredDFlipFlop(&counter->flip_flops[0], q_bar_0, HIGH);
    
    // toggle remaining FFs on falling edges
    for(int i = 1; i < 8; i++){
        bit prev_new_state = counter->flip_flops[i-1].slave.rs_ff.q;
        
        if(saved_q[i-1] == HIGH && prev_new_state == LOW){
            bit current_q_bar = counter->flip_flops[i].slave.rs_ff.q_bar;
            edgeTriggeredDFlipFlop(&counter->flip_flops[i], current_q_bar, LOW);
            edgeTriggeredDFlipFlop(&counter->flip_flops[i], current_q_bar, HIGH);
        }
    }
}

/**
 * @brief reads the current counter value
 * 
 * @param counter pointer to the counter
 * @return byte the current count (0-255)
 * 
 * FF0 is LSB, FF7 is MSB
 */
byte getEightBitCounterValue(eight_bit_counter* counter){
    byte value;
    for(int i = 0; i < 8; i++){
        value.bits[i] = counter->flip_flops[7-i].slave.rs_ff.q;
    }
    return value;
}

/**
 * @brief resets the counter back to 0
 * 
 * @param counter pointer to the counter to reset
 */
void resetEightBitCounter(eight_bit_counter* counter){
    for(int i = 0; i < 8; i++){
        counter->flip_flops[i] = newEdgeTriggeredDFlipFlop();
    }
}

/**
 * @brief initializes a new 16-bit counter starting at 0
 *
 * @return sixteen_bit_counter the initialized counter
 *
 * uses edge_triggered_d_flip_flop_pc for all 16 flip-flops so that
 * the Jump instruction can preset any bit to 0 or 1 via the clear/preset
 * inputs, loading an arbitrary 16-bit address into the counter.
 * FF0 is LSB (bit 0), FF15 is MSB (bit 15).
 */
sixteen_bit_counter newSixteenBitCounter(void) {
    sixteen_bit_counter counter;
    for(int i = 0; i < 16; i++) {
        counter.flip_flops[i] = newEdgeTriggeredDFlipFlopPC();
    }
    return counter;
}

/**
 * @brief increments the counter by one (one clock tick)
 *
 * @param counter pointer to the counter
 *
 * same ripple design as the 8-bit counter, extended to 16 stages.
 * FF0 toggles every tick; each subsequent FF toggles on the falling
 * edge of the previous FF's Q output.
 */
void tickSixteenBitCounter(sixteen_bit_counter* counter) {
    bit saved_q[16];
    for(int i = 0; i < 16; i++) {
        saved_q[i] = counter->flip_flops[i].q;
    }
 
    bit q_bar_0 = counter->flip_flops[0].q_bar;
    edgeTriggeredDFlipFlopPC(&counter->flip_flops[0], q_bar_0, LOW, LOW, LOW);
    edgeTriggeredDFlipFlopPC(&counter->flip_flops[0], q_bar_0, HIGH, LOW, LOW);
 
    for(int i = 1; i < 16; i++) {
        bit prev_new_q = counter->flip_flops[i-1].q;
        if(saved_q[i-1] == HIGH && prev_new_q == LOW) {
            bit q_bar_i = counter->flip_flops[i].q_bar;
            edgeTriggeredDFlipFlopPC(&counter->flip_flops[i], q_bar_i, LOW, LOW, LOW);
            edgeTriggeredDFlipFlopPC(&counter->flip_flops[i], q_bar_i, HIGH, LOW, LOW);
        }
    }
}

/**
 * @brief reads the current 16-bit counter value as two hex bytes
 *
 * @param counter pointer to the counter
 * @param hi      output: high byte (bits 15–8) as a byte struct
 * @param lo      output: low  byte (bits  7–0) as a byte struct
 *
 * FF15 is MSB, FF0 is LSB.
 * hi->bits[0] is bit15, hi->bits[7] is bit8.
 * lo->bits[0] is bit7,  lo->bits[7] is bit0.
 */
void getSixteenBitCounterValue(sixteen_bit_counter* counter, byte* hi, byte* lo) {
    for(int i = 0; i < 8; i++) {
        hi->bits[i] = counter->flip_flops[15 - i].q;
        lo->bits[i] = counter->flip_flops[7  - i].q;
    }
}

/**
 * @brief loads a 16-bit address into the counter (used by Jump)
 *
 * @param counter  pointer to the counter
 * @param addr_hi  high byte of the target address, e.g. "0x20"
 * @param addr_lo  low  byte of the target address, e.g. "0x00"
 *
 * uses the preset/clear inputs of each flip-flop to force every bit
 * to the required value, exactly as the book describes for the Jump
 * instruction. after this call the counter will continue incrementing
 * from the loaded address on the next tick.
 */
void jumpSixteenBitCounter(sixteen_bit_counter* counter, char* addr_hi, char* addr_lo) {
    byte hi = hexToByte(addr_hi);
    byte lo = hexToByte(addr_lo);
 
    bit target[16];
    for(int i = 0; i < 8; i++) {
        target[i] = lo.bits[7 - i];
        target[i + 8] = hi.bits[7 - i];
    }
 
    for(int i = 0; i < 16; i++) {
        bit preset = target[i];
        bit clear = notGate(target[i]);
        edgeTriggeredDFlipFlopPC(&counter->flip_flops[i],
                                 LOW, LOW, preset, clear);
    }
}

/**
 * @brief resets the program counter back to 0x0000
 *
 * @param counter pointer to the counter
 */
void resetSixteenBitCounter(sixteen_bit_counter* counter) {
    for(int i = 0; i < 16; i++) {
        counter->flip_flops[i] = newEdgeTriggeredDFlipFlopPC();
    }
}