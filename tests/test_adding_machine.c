/**
 * @file test_adding_machine.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for the adding machine: accumulate, reset, no-save, overflow.
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/logic.h"
#include "test_framework.h"

/* Helper: compare two bytes, report failure per-bit with a label prefix */
static void assert_bytes_eq(byte expected, byte actual, const char* label) {
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(expected.bits[i], actual.bits[i], label);
}

static void test_add_to_zero(void) {
    adding_machine calc = newAddingMachine();
    byte five = hexToByte("0x05");

    /* from_latch=LOW means add input to zero */
    runAddingMachine(&calc, five, LOW, HIGH);
    byte out = eightBitLatchOutput(&calc.latch);
    assert_bytes_eq(five, out, "add 5 to zero");
}

static void test_accumulate(void) {
    adding_machine calc = newAddingMachine();
    byte five  = hexToByte("0x05");
    byte three = hexToByte("0x03");
    byte eight = hexToByte("0x08");

    runAddingMachine(&calc, five,  LOW,  HIGH);  /* 0 + 5 = 5, save  */
    runAddingMachine(&calc, three, HIGH, HIGH);  /* 5 + 3 = 8, save  */
    assert_bytes_eq(eight, getAddingMachineSum(&calc), "5+3=8 accumulate");
}

static void test_no_save(void) {
    adding_machine calc = newAddingMachine();
    byte five  = hexToByte("0x05");
    byte seven = hexToByte("0x07");
    byte twelve = hexToByte("0x0C");

    runAddingMachine(&calc, five,  LOW,  HIGH);  /* save 5 */
    runAddingMachine(&calc, seven, HIGH, LOW);   /* compute 5+7 but don't save */

    /* sum should be 12 */
    assert_bytes_eq(twelve, getAddingMachineSum(&calc), "no-save sum=12");

    /* latch should still hold 5 */
    assert_bytes_eq(five, eightBitLatchOutput(&calc.latch), "no-save latch still 5");
}

static void test_reset(void) {
    adding_machine calc = newAddingMachine();
    byte hundred = hexToByte("0x64");
    byte twenty  = hexToByte("0x14");

    runAddingMachine(&calc, hundred, LOW,  HIGH);  /* save 100 */
    runAddingMachine(&calc, twenty,  HIGH, HIGH);  /* 100+20 = 120 */
    runAddingMachine(&calc, twenty,  LOW,  HIGH);  /* reset: 0+20 = 20 */

    assert_bytes_eq(twenty, eightBitLatchOutput(&calc.latch), "reset to 20");
}

static void test_overflow(void) {
    adding_machine calc = newAddingMachine();
    /* 200 + 100 = 300, wraps to 44 (300 - 256) */
    byte twohundred = hexToByte("0xC8");
    byte hundred    = hexToByte("0x64");
    byte forty_four = hexToByte("0x2C");

    runAddingMachine(&calc, twohundred, LOW,  HIGH);
    runAddingMachine(&calc, hundred,    HIGH, LOW);   /* compute, don't save */
    assert_bytes_eq(forty_four, getAddingMachineSum(&calc), "200+100 overflow=44");
}

int main(void) {
    test_add_to_zero();
    test_accumulate();
    test_no_save();
    test_reset();
    test_overflow();
    PRINT_SUMMARY("test_adding_machine");
    return _tests_failed;
}