#include "logic.h"
int printf(const char* format, ...);

int main(){
    // Initialize the adding machine
    adding_machine calc = newAddingMachine();
    printf("\n*** ADDING MACHINE TEST PROGRAM ***\n");

    // Test 1: Add 5 to empty machine (should get 5)
    printf("\n=== Test 1: Add 5 to empty machine ===\n");
    byte five = {{0,0,0,0,0,1,0,1}};  // 5 in binary
    runAddingMachine(&calc, five, LOW, HIGH);  // from_latch=LOW (add to 0), save=HIGH
    printf("Result saved in latch: ");
    printByte(eightBitLatchOutput(&calc.latch));

    // Test 2: Add 3 to the saved 5 (should get 8)
    printf("\n=== Test 2: Accumulate - Add 3 to saved 5 ===\n");
    byte three = {{0,0,0,0,0,0,1,1}};  // 3 in binary
    runAddingMachine(&calc, three, HIGH, HIGH);  // from_latch=HIGH (accumulate), save=HIGH
    printf("5 + 3 = ");
    printByte(getAddingMachineSum(&calc));
    printf("Latch now contains: ");
    printByte(eightBitLatchOutput(&calc.latch));

    // Test 3: Add 10 to the saved 8 (should get 18)
    printf("\n=== Test 3: Add 10 to saved 8 ===\n");
    byte ten = {{0,0,0,0,1,0,1,0}};  // 10 in binary
    runAddingMachine(&calc, ten, HIGH, HIGH);  // accumulate and save
    printf("8 + 10 = ");
    printByte(getAddingMachineSum(&calc));

    // Test 4: Compute without saving (latch should stay at 18)
    printf("\n=== Test 4: Compute 18 + 7 but DON'T save ===\n");
    byte seven = {{0,0,0,0,0,1,1,1}};  // 7 in binary
    runAddingMachine(&calc, seven, HIGH, LOW);  // from_latch=HIGH, save=LOW (don't save)
    printf("Computed sum: ");
    printByte(getAddingMachineSum(&calc));
    printf("Latch still has: ");
    printByte(eightBitLatchOutput(&calc.latch));

    // Test 5: Reset - add 100 to zero (not from latch)
    printf("\n=== Test 5: Reset - Start fresh with 100 ===\n");
    byte hundred = {{0,1,1,0,0,1,0,0}};  // 100 in binary
    runAddingMachine(&calc, hundred, LOW, HIGH);  // from_latch=LOW (reset), save=HIGH
    printf("Fresh start with 100: ");
    printByte(eightBitLatchOutput(&calc.latch));

    // Test 6: Chain of additions
    printf("\n=== Test 6: Chain - 100 + 20 + 30 ===\n");
    byte twenty = {{0,0,0,1,0,1,0,0}};  // 20 in binary
    runAddingMachine(&calc, twenty, HIGH, HIGH);
    printf("After +20: ");
    printByte(getAddingMachineSum(&calc));
    
    byte thirty = {{0,0,0,1,1,1,1,0}};  // 30 in binary
    runAddingMachine(&calc, thirty, HIGH, HIGH);
    printf("After +30: ");
    printByte(getAddingMachineSum(&calc));
    printf("Final total: ");
    printByte(eightBitLatchOutput(&calc.latch));

    // Test 7: Overflow test (200 + 100 = 300, but max is 255)
    printf("\n=== Test 7: Overflow - 200 + 100 ===\n");
    byte twohundred = {{1,1,0,0,1,0,0,0}};  // 200 in binary
    runAddingMachine(&calc, twohundred, LOW, HIGH);  // reset to 200
    printf("Reset to 200: ");
    printByte(eightBitLatchOutput(&calc.latch));
    
    runAddingMachine(&calc, hundred, HIGH, LOW);  // compute 200+100 (don't save)
    printf("200 + 100 = (overflows to): ");
    printByte(getAddingMachineSum(&calc));

    // Test 8: Adding zero
    printf("\n=== Test 8: Add zero ===\n");
    byte zero = {{0,0,0,0,0,0,0,0}};  // 0 in binary
    runAddingMachine(&calc, five, LOW, HIGH);  // reset to 5
    printf("Reset to 5: ");
    printByte(eightBitLatchOutput(&calc.latch));
    
    runAddingMachine(&calc, zero, HIGH, HIGH);  // add 0
    printf("5 + 0 = ");
    printByte(getAddingMachineSum(&calc));

    printf("\n*** ALL TESTS COMPLETE ***\n\n");
    return 0;
}