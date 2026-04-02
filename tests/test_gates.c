/**
 * @file test_gates.c
 * @author zenze (zenzesama@gmail.com)
 * @brief Unit tests for all logic gates (AND, OR, NOT, NAND, NOR, XOR)
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"
#include "test_framework.h"

static void test_and_gate(void) {
    ASSERT_BIT_EQ(LOW,  andGate(LOW,  LOW),  "AND 0,0");
    ASSERT_BIT_EQ(LOW,  andGate(LOW,  HIGH), "AND 0,1");
    ASSERT_BIT_EQ(LOW,  andGate(HIGH, LOW),  "AND 1,0");
    ASSERT_BIT_EQ(HIGH, andGate(HIGH, HIGH), "AND 1,1");
}
 
static void test_or_gate(void) {
    ASSERT_BIT_EQ(LOW,  orGate(LOW,  LOW),  "OR 0,0");
    ASSERT_BIT_EQ(HIGH, orGate(LOW,  HIGH), "OR 0,1");
    ASSERT_BIT_EQ(HIGH, orGate(HIGH, LOW),  "OR 1,0");
    ASSERT_BIT_EQ(HIGH, orGate(HIGH, HIGH), "OR 1,1");
}
 
static void test_not_gate(void) {
    ASSERT_BIT_EQ(HIGH, notGate(LOW),  "NOT 0");
    ASSERT_BIT_EQ(LOW,  notGate(HIGH), "NOT 1");
}
 
static void test_nand_gate(void) {
    ASSERT_BIT_EQ(HIGH, nandGate(LOW,  LOW),  "NAND 0,0");
    ASSERT_BIT_EQ(HIGH, nandGate(LOW,  HIGH), "NAND 0,1");
    ASSERT_BIT_EQ(HIGH, nandGate(HIGH, LOW),  "NAND 1,0");
    ASSERT_BIT_EQ(LOW,  nandGate(HIGH, HIGH), "NAND 1,1");
}
 
static void test_nor_gate(void) {
    ASSERT_BIT_EQ(HIGH, norGate(LOW,  LOW),  "NOR 0,0");
    ASSERT_BIT_EQ(LOW,  norGate(LOW,  HIGH), "NOR 0,1");
    ASSERT_BIT_EQ(LOW,  norGate(HIGH, LOW),  "NOR 1,0");
    ASSERT_BIT_EQ(LOW,  norGate(HIGH, HIGH), "NOR 1,1");
}
 
static void test_xor_gate(void) {
    ASSERT_BIT_EQ(LOW,  xorGate(LOW,  LOW),  "XOR 0,0");
    ASSERT_BIT_EQ(HIGH, xorGate(LOW,  HIGH), "XOR 0,1");
    ASSERT_BIT_EQ(HIGH, xorGate(HIGH, LOW),  "XOR 1,0");
    ASSERT_BIT_EQ(LOW,  xorGate(HIGH, HIGH), "XOR 1,1");
}
 
int main(void) {
    test_and_gate();
    test_or_gate();
    test_not_gate();
    test_nand_gate();
    test_nor_gate();
    test_xor_gate();
    PRINT_SUMMARY("test_gates");
    return _tests_failed;
}