#ifndef LOGIC_H
#define LOGIC_H

typedef enum {LOW = 0, HIGH = 1} bit;

typedef struct{
    bit bits[8];
} byte;

bit andGate(bit a, bit b);
bit orGate(bit a, bit b);
bit notGate(bit a);
bit nandGate(bit a, bit b);
bit norGate(bit a, bit b);
bit xorGate(bit a, bit b);

void printBit(bit a);
void printByte(byte byte_a);

typedef struct{
    bit sum_out;
    bit carry_out;
} half_adder;

typedef struct{
    bit sum_out;
    bit carry_out;
} full_adder;

half_adder halfAdder(bit a, bit b);
full_adder fullAdder(bit a, bit b, bit carry_in);
byte eightBitAdder(byte byte_a, byte byte_b, bit carry, bit* carry_out);
byte notByte(byte byte_a);
byte eightBitSubtractor(byte byte_a, byte byte_b, bit borrow_in, bit* borrow_out);


typedef struct{
    bit q;
    bit q_bar;
}rs_flip_flop;

rs_flip_flop newRsFlipFlop(void);
void rsFlipFlop(rs_flip_flop* ff, bit r, bit s);


typedef struct{
    rs_flip_flop rs_ff;
}d_flip_flop;

d_flip_flop newDFlipFlop(void);
void dFlipFlop(d_flip_flop* ff, bit data, bit clock);


typedef struct{
    d_flip_flop flip_flops[8];
}eight_bit_latch;

eight_bit_latch newEightBitLatch(void);
void eightBitLatch(eight_bit_latch* latch, byte data, bit clock);
byte eightBitLatchOutput(eight_bit_latch* latch);


bit twoToOneSelector(bit a, bit b, bit select);
byte eightBitTwoToOneSelector(byte a, byte b, bit select);

typedef struct {
    eight_bit_latch latch;
    byte last_sum;
}adding_machine;

adding_machine newAddingMachine(void);
void runAddingMachine(adding_machine* calc, byte input, bit from_latch, bit save);
byte getAddingMachineSum(adding_machine* calc);


typedef struct {
    d_flip_flop master;
    d_flip_flop slave;
    bit last_clock;
}edge_triggered_d_flip_flop;

edge_triggered_d_flip_flop newEdgeTriggeredDFlipFlop(void);
void edgeTriggeredDFlipFlop(edge_triggered_d_flip_flop* ff, bit data, bit clock);

typedef struct{
    edge_triggered_d_flip_flop base_ff;
    bit q;
    bit q_bar;
}edge_triggered_d_flip_flop_pc;

edge_triggered_d_flip_flop_pc newEdgeTriggeredDFlipFlopPC(void);
void edgeTriggeredDFlipFlopPC(edge_triggered_d_flip_flop_pc* ff, bit data, bit clock, bit preset, bit clear);

byte hexToByte(char *hex);
int byteToDecimal(byte b);

typedef struct{
    edge_triggered_d_flip_flop flip_flops[8];
}eight_bit_counter;

eight_bit_counter newEightBitCounter(void);
void tickEightBitCounter(eight_bit_counter* counter);
byte getEightBitCounterValue(eight_bit_counter* counter);
void resetEightBitCounter(eight_bit_counter* counter);

#endif