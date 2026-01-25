typedef enum {LOW = 0, HIGH = 1} bit;
extern int putchar(int);

bit and_gate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, LOW},
        {LOW, HIGH}
    };
    return tt[a][b];
}

bit or_gate(bit a, bit b){
    static const bit tt[2][2] = {
        {LOW, HIGH},
        {HIGH, HIGH}
    };
    return tt[a][b];
}

bit not_gate(bit a){
    static const bit tt[2] = {HIGH, LOW};
    return tt[a];
}

bit nand_gate(bit a, bit b){
    return not_gate(and_gate(a, b));
}

bit nor_gate(bit a, bit b){
    return not_gate(or_gate(a, b));
}

bit xor_gate(bit a, bit b){
    return and_gate(or_gate(a, b), nand_gate(a, b));
}

void print_bit(bit a){
    putchar(a ? '1' :'0');
}


typedef struct{
    bit sum;
    bit carry;
} halfAdder;

halfAdder half_adder(bit a, bit b){
    halfAdder result;
    result.carry = and_gate(a, b);
    result.sum = xor_gate(a, b);

    return result;
}

int main(){
    bit input_a = HIGH;
    bit input_b = LOW;

    halfAdder result = half_adder(input_a, input_b);

    putchar('C'); putchar('='); print_bit(result.carry);
    putchar('\n');
    putchar('S'); putchar('='); print_bit(result.sum);
    putchar('\n');
    return 0;
}