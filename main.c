#include "logic.h"
int printf(const char* format, ...);

int main(){
    adding_machine calc = newAddingMachine();

    // Test 1: Basic addition to empty latch
    byte five = {{0,0,0,0,0,1,0,1}}; // this is 5
    printf("\n=== Test 1: Adding 5 to empty latch ===\n");
    runAddingMachine(&calc, five, LOW, LOW);
    byte latch_out = eightBitLatchOutput(&calc.latch);
    printf("Latch contents (before save): ");
    printByte(latch_out);
    byte result = getAddingMachineSum(&calc);
    printf("Sum: ");
    printByte(result);

    printf("\nSAVE the result:\n");
    runAddingMachine(&calc, five, LOW, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("Latch contents (after save): ");
    printByte(latch_out);

    // Test 2: Accumulate (add to saved value)
    printf("\n=== Test 2: Adding 3 to the saved 5 ===\n");
    byte three = {{0,0,0,0,0,0,1,1}}; // this is 3
    runAddingMachine(&calc, three, HIGH, LOW);
    result = getAddingMachineSum(&calc);
    printf("Sum: ");
    printByte(result);

    printf("\nSAVE the 8:\n");
    runAddingMachine(&calc, three, HIGH, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("Latch contents: ");
    printByte(latch_out);

    // Test 3: Continue accumulating
    printf("\n=== Test 3: Adding 10 to the saved 8 ===\n");
    byte ten = {{0,0,0,0,1,0,1,0}}; // this is 10
    runAddingMachine(&calc, ten, HIGH, LOW);
    result = getAddingMachineSum(&calc);
    printf("Sum: ");
    printByte(result);
    
    runAddingMachine(&calc, ten, HIGH, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("Latch saved: ");
    printByte(latch_out);

    // Test 4: Reset by adding to zero (not selecting from latch)
    printf("\n=== Test 4: Reset - Adding 7 (not from latch) ===\n");
    byte seven = {{0,0,0,0,0,1,1,1}}; // this is 7
    runAddingMachine(&calc, seven, LOW, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("Latch contents (should be 7): ");
    printByte(latch_out);

    // Test 5: Chain of accumulations
    printf("\n=== Test 5: Chain additions: 7 + 15 + 20 ===\n");
    byte fifteen = {{0,0,0,0,1,1,1,1}}; // this is 15
    runAddingMachine(&calc, fifteen, HIGH, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("After +15: ");
    printByte(latch_out);

    byte twenty = {{0,0,0,1,0,1,0,0}}; // this is 20
    runAddingMachine(&calc, twenty, HIGH, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("After +20: ");
    printByte(latch_out);

    // Test 6: Overflow test
    printf("\n=== Test 6: Overflow test ===\n");
    byte twohundred = {{1,1,0,0,1,0,0,0}}; // this is 200
    runAddingMachine(&calc, twohundred, LOW, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("Saved 200: ");
    printByte(latch_out);

    byte hundred = {{0,1,1,0,0,1,0,0}}; // this is 100
    runAddingMachine(&calc, hundred, HIGH, LOW);
    result = getAddingMachineSum(&calc);
    printf("200 + 100 = (should overflow): ");
    printByte(result);

    // Test 7: Adding zero
    printf("\n=== Test 7: Adding zero ===\n");
    byte zero = {{0,0,0,0,0,0,0,0}}; // this is 0
    runAddingMachine(&calc, five, LOW, HIGH); // Reset to 5
    printf("Reset to 5: ");
    printByte(eightBitLatchOutput(&calc.latch));
    
    runAddingMachine(&calc, zero, HIGH, HIGH);
    latch_out = eightBitLatchOutput(&calc.latch);
    printf("After adding 0: ");
    printByte(latch_out);

    // Test 8: Multiple operations without saving
    printf("\n=== Test 8: Compute without saving ===\n");
    byte two = {{0,0,0,0,0,0,1,0}}; // this is 2
    runAddingMachine(&calc, two, LOW, HIGH); // Save 2
    printf("Saved 2: ");
    printByte(eightBitLatchOutput(&calc.latch));

    runAddingMachine(&calc, three, HIGH, LOW); // Compute 2+3 but don't save
    result = getAddingMachineSum(&calc);
    printf("Computed 2+3=5: ");
    printByte(result);
    printf("Latch still has 2: ");
    printByte(eightBitLatchOutput(&calc.latch));

    byte four = {{0,0,0,0,0,1,0,0}}; // this is 4
    runAddingMachine(&calc, four, HIGH, LOW); // Compute 2+4 but don't save
    result = getAddingMachineSum(&calc);
    printf("Computed 2+4=6: ");
    printByte(result);
    printf("Latch still has 2: ");
    printByte(eightBitLatchOutput(&calc.latch));

    return 0;
}