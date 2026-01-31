#include "logic.h"
int printf(const char* format, ...);

int main(){
    byte byte_a = {{0,0,0,0,1,0,0,0}}; // this is 8
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
    
    return 0;
}