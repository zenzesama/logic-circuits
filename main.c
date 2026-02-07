#include "logic.h"
int printf(const char* format, ...);

int main(){
/*
    byte byte_a = {{0,0,0,0,1,1,0,0}}; // this is 12
    byte byte_b = {{0,0,0,0,0,1,0,0}}; // this is 4
    bit carry_out, borrow_out;

    byte sum = eightBitAdder(byte_a, byte_b, LOW, &carry_out);
    byte diff = eightBitSubtractor(byte_a, byte_b, LOW, &borrow_out);

    if(carry_out){
        printf("Addition overflowed!\n");
    }
    else{
        printByte(sum);
    }

    if(borrow_out){
        printf("Subtraction underflowed!\n");
    }
    else{
        printByte(diff);
    }
*/
    rs_flip_flop rs_ff_1 = newRsFlipFlop();
    printf("Q ~Q\n");
    printf("%d  %d\n", rs_ff_1.q, rs_ff_1.q_bar);
    rsFlipFlop(&rs_ff_1, LOW, HIGH);
    printf("%d  %d\n", rs_ff_1.q, rs_ff_1.q_bar);
    rsFlipFlop(&rs_ff_1, HIGH, LOW);
    printf("%d  %d\n", rs_ff_1.q, rs_ff_1.q_bar);
    rsFlipFlop(&rs_ff_1, LOW, LOW);
    printf("%d  %d\n", rs_ff_1.q, rs_ff_1.q_bar);
    rsFlipFlop(&rs_ff_1, HIGH, LOW);
    printf("%d  %d\n", rs_ff_1.q, rs_ff_1.q_bar);
    return 0;
}