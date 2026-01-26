typedef enum {LOW = 0, HIGH = 1} bit;
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


typedef struct{
    bit sum_out;
    bit carry_out;
} half_adder;

half_adder halfAdder(bit a, bit b){
    half_adder result;
    result.carry_out = andGate(a, b);
    result.sum_out = xorGate(a, b);
    return result;
}

typedef struct{
    bit sum_out;
    bit carry_out;
} full_adder;

full_adder fullAdder(bit a, bit b, bit carry_in){
    full_adder result;
    half_adder halfAdder1, halfAdder2;

    halfAdder1 = halfAdder(a, b);
    halfAdder2 = halfAdder(halfAdder1.sum_out, carry_in);

    result.sum_out = halfAdder2.sum_out;
    result.carry_out = orGate(halfAdder1.carry_out, halfAdder2.carry_out);

    return result;
}

typedef struct{
    bit bits[8];
} byte;

byte eightBitAdder(byte byte_a, byte byte_b, bit carry){
    byte sum;
    for(int i = 0; i < 8; i++){
        full_adder result = fullAdder(byte_a.bits[i], byte_b.bits[i], carry);
        sum.bits[i] = result.sum_out;
        carry = result.carry_out;
    }
    return sum;
}

void printByte(byte byte_a){
    for(int i = 7; i >= 0; i--){
        printBit(byte_a.bits[i]);
    }
}

int main(){
    bit input_a = HIGH;
    bit input_b = HIGH;
    bit carry_in = LOW;

    byte byte_a, byte_b;

    byte_a = {1,0,1,0,1,0,1,0};

    full_adder result = fullAdder(input_a, input_b, carry_in);

    putchar('C'); putchar('='); printBit(result.carry_out);
    putchar('\n');
    putchar('S'); putchar('='); printBit(result.sum_out);
    putchar('\n');
    return 0;
}