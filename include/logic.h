#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>

typedef uint8_t bit;
#define LOW ((bit)0)
#define HIGH ((bit)1)

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
void byteToHex(byte b, char out[5]);

typedef struct{
    edge_triggered_d_flip_flop flip_flops[8];
}eight_bit_counter;

eight_bit_counter newEightBitCounter(void);
void tickEightBitCounter(eight_bit_counter* counter);
byte getEightBitCounterValue(eight_bit_counter* counter);
void resetEightBitCounter(eight_bit_counter* counter);


void decoder3to8(bit s0, bit s1, bit s2, bit out[8]);
bit eightToOneSelector(bit in[8], bit s0, bit s1, bit s2);

typedef struct{
    d_flip_flop latches[8];
}ram8x1;

ram8x1 newRam8x1(void);
bit ram8x1Operation(ram8x1* ram, byte addr, bit data_in, bit write);

typedef struct{
    ram8x1 lanes[8];
}ram8x8;
 
ram8x8 newRam8x8(void);
void writeRam8x8(ram8x8* ram, char* addr, byte data);
byte readRam8x8(ram8x8* ram, char* addr);


typedef struct {
    d_flip_flop bits[8];
} ram_cell;
 
typedef struct {
    ram_cell cells[65536];
} ram64kx8;
 
ram64kx8* newRam64kx8(void);
void writeRam64kx8(ram64kx8* ram, char* addr_hi, char* addr_lo, byte data);
byte readRam64kx8(ram64kx8* ram, char* addr_hi, char* addr_lo);


typedef struct {
    edge_triggered_d_flip_flop_pc flip_flops[16];
} sixteen_bit_counter;

sixteen_bit_counter newSixteenBitCounter(void);
void tickSixteenBitCounter(sixteen_bit_counter* counter);
void getSixteenBitCounterValue(sixteen_bit_counter* counter, byte* hi, byte* lo);
void jumpSixteenBitCounter(sixteen_bit_counter* counter, char* addr_hi, char* addr_lo);
void resetSixteenBitCounter(sixteen_bit_counter* counter);


typedef struct {
    eight_bit_latch accumulator;
    d_flip_flop carry_latch;
    d_flip_flop zero_latch;
} alu;

alu newALU(void);
void aluLoad(alu* a, byte data);
byte aluGetAccumulator(alu* a);
bit aluGetCarry(alu* a);
bit aluGetZero(alu* a);
void runALU(alu* a, char* opcode, byte operand);


typedef struct {
    eight_bit_latch opcode;
    eight_bit_latch addr_hi;
    eight_bit_latch addr_lo;
} instruction_register;
 
instruction_register newInstructionRegister(void);
void loadIR(instruction_register* ir, char* opcode, char* addr_hi, char* addr_lo);
byte irGetOpcode(instruction_register* ir);
byte irGetAddrHi(instruction_register* ir);
byte irGetAddrLo(instruction_register* ir);
int  irGetAddress(instruction_register* ir);


typedef enum {
    CU_RUNNING = 0,
    CU_HALTED  = 1
} cu_status;
 
typedef struct {
    sixteen_bit_counter *pc;
    ram64kx8 *ram;
    instruction_register *ir;
    alu *a;
    int halted;
} control_unit;
 
control_unit* newCU(void);
void freeCU(control_unit* cu);
void loadProgram(control_unit* cu, int* program, int length);
cu_status stepCU(control_unit* cu);


typedef struct {
    control_unit* cu;
    int cycles;
    int debug;
} cpu;

cpu* newCPU(void);
void freeCPU(cpu* c);
void resetCPU(cpu* c);
void cpuSetDebug(cpu* c, int debug);
void cpuLoad(cpu* c, int* program, int length);
byte cpuGetAccumulator(cpu* c);
int cpuGetPC(cpu* c);
bit cpuGetCarry(cpu* c);
bit cpuGetZero(cpu* c);
byte cpuReadRAM(cpu* c, int addr);
int cpuGetCycles(cpu* c);
cu_status stepCPU(cpu* c);
int runCPU(cpu* c);

#endif