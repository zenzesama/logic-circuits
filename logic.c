#include "logic.h"

extern int putchar(int);

bit andGate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, LOW},
        {LOW, HIGH}
    };
    return tt[a][b];
}

bit orGate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, HIGH},
        {HIGH, HIGH}
    };
    return tt[a][b];
}

bit notGate(bit a){
    static const bit tt[2] = {HIGH, LOW};
    return tt[a];
}

bit nandGate(bit a, bit b){
    return notGate(andGate(a, b));
}

bit norGate(bit a, bit b){
    return notGate(orGate(a, b));
}

bit xorGate(bit a, bit b){
    return andGate(orGate(a, b), nandGate(a, b));
}

void printBit(bit a){
    putchar(a ? '1' : '0');
}

void printByte(byte byte_a){ // this makes it so that the MSB is at left side, "human readable format!"
    for (int i = 0; i < 8; i++){
        printBit(byte_a.bits[i]);
    }
    putchar('\n');
}


half_adder halfAdder(bit a, bit b){
    half_adder result;
    result.carry_out = andGate(a, b);
    result.sum_out = xorGate(a, b);
    return result;
}

full_adder fullAdder(bit a, bit b, bit carry_in){
    full_adder result;
    half_adder h1 = halfAdder(a, b);
    half_adder h2 = halfAdder(h1.sum_out, carry_in);

    result.sum_out = h2.sum_out;
    result.carry_out = orGate(h1.carry_out, h2.carry_out);

    return result;
}

byte eightBitAdder(byte byte_a, byte byte_b, bit carry, bit* carry_out){
    byte sum;

    for(int i = 7; i >= 0; i--){ // this makes it so that the MSB is in right side, "computer loop format!"
        full_adder result = fullAdder(byte_a.bits[i], byte_b.bits[i], carry);
        sum.bits[i] = result.sum_out;
        carry = result.carry_out;
    }

    *carry_out = carry;
    return sum;
}

byte notByte(byte byte_a){
    byte result;
    for(int i = 0; i < 8; i++){
        result.bits[i] = notGate(byte_a.bits[i]);
    }
    return result;
}

byte eightBitSubtractor(byte byte_a, byte byte_b, bit borrow_in, bit* borrow_out){
    byte b_invert = notByte(byte_b);
    bit carry_out;

    bit carry_in = notGate(borrow_in);

    byte result = eightBitAdder(byte_a, b_invert, carry_in, &carry_out);

    *borrow_out = notGate(carry_out);
    return result;
}


rs_flip_flop newRsFlipFlop(){
    rs_flip_flop ff = {LOW, HIGH};
    return ff;
}

void rsFlipFlop(rs_flip_flop* ff, bit r, bit s){
    bit q, q_bar;
    do{
        q = norGate(r, ff->q_bar);
        q_bar = norGate(s, q);
        if(q == ff->q && q_bar == ff->q_bar) break;
        ff->q = q;
        ff->q_bar = q_bar;
    }while(1);
}


d_flip_flop newDFlipFlop(){
    d_flip_flop ff;
    ff.rs_ff = newRsFlipFlop();
    return ff;
}

void dFlipFlop(d_flip_flop* ff, bit data, bit clock){
    bit s = andGate(clock, data);
    bit r = andGate(clock, notGate(data));

    rsFlipFlop(&ff->rs_ff, r, s);
}


eight_bit_latch newEightBitLatch(void){
    eight_bit_latch latch;
    for(int i = 0; i < 8; i++){
        latch.flip_flops[i] = newDFlipFlop();
    }
    return latch;
}

void eightBitLatch(eight_bit_latch* latch, byte data, bit clock){
    for(int i = 0; i < 8; i++){
        dFlipFlop(&latch->flip_flops[i], data.bits[i], clock);
    }
}

byte eightBitLatchOutput(eight_bit_latch* latch){
    byte output;
    for(int i = 0; i < 8; i++){
        output.bits[i] = latch->flip_flops[i].rs_ff.q;
    }
    return output;
}


bit twoToOneSelector(bit a, bit b, bit select){
    bit notSelect = notGate(select);
    bit a_path = andGate(a, notSelect);
    bit b_path = andGate(b, select);
    return orGate(a_path, b_path);
}

byte eightBitTwoToOneSelector(byte a, byte b, bit select){
    byte output;
    for(int i = 0; i < 8; i++){
        output.bits[i] = twoToOneSelector(a.bits[i], b.bits[i], select);
    }
    return output;
}

adding_machine newAddingMachine(void){
    adding_machine machine;
    machine.latch = newEightBitLatch();
    byte zero = {{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW}};
    machine.switches_input = zero;
    machine.select_switch = LOW;
    machine.save_switch = LOW;
    return machine;
}

void runAddingMachine(adding_machine* machine, byte switches_input, bit select_from_latch, bit save){
    machine->switches_input = switches_input;
    machine->select_switch = select_from_latch;

    byte latch_output = eightBitLatchOutput(&machine->latch);
    byte zero = {{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW}};
    byte selector_output = eightBitTwoToOneSelector(zero, latch_output, select_from_latch);
    bit carry_out;
    byte sum = eightBitAdder(switches_input, selector_output, LOW, &carry_out);
    eightBitLatch(&machine->latch, sum, save);
}

byte getAddingMachineSum(adding_machine* machine){
    byte switches = machine->switches_input;
    byte latch_output = eightBitLatchOutput(&machine->latch);
    byte zero = {{LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW}};
    byte selector_output = eightBitTwoToOneSelector(zero, latch_output, machine->select_switch);

    bit carry_out;
    return eightBitAdder(switches, selector_output, LOW, &carry_out);
}