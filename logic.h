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

#endif