/**
 * @file test_adders.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for half adder, full adder, 8-bit adder, and 8-bit subtractor.
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "logic.h"
#include "test_framework.h"

static void test_half_adder(void) {
    half_adder r;

    r = halfAdder(LOW, LOW);
    ASSERT_BIT_EQ(LOW, r.sum_out,   "HA 0+0 sum");
    ASSERT_BIT_EQ(LOW, r.carry_out, "HA 0+0 carry");

    r = halfAdder(LOW, HIGH);
    ASSERT_BIT_EQ(HIGH, r.sum_out,  "HA 0+1 sum");
    ASSERT_BIT_EQ(LOW,  r.carry_out,"HA 0+1 carry");

    r = halfAdder(HIGH, LOW);
    ASSERT_BIT_EQ(HIGH, r.sum_out,  "HA 1+0 sum");
    ASSERT_BIT_EQ(LOW,  r.carry_out,"HA 1+0 carry");

    r = halfAdder(HIGH, HIGH);
    ASSERT_BIT_EQ(LOW,  r.sum_out,  "HA 1+1 sum");
    ASSERT_BIT_EQ(HIGH, r.carry_out,"HA 1+1 carry");
}

static void test_full_adder(void) {
    full_adder r;

    /* 0+0+0 = 0, carry 0 */
    r = fullAdder(LOW, LOW, LOW);
    ASSERT_BIT_EQ(LOW, r.sum_out,   "FA 0+0+0 sum");
    ASSERT_BIT_EQ(LOW, r.carry_out, "FA 0+0+0 carry");

    /* 1+1+0 = 0, carry 1 */
    r = fullAdder(HIGH, HIGH, LOW);
    ASSERT_BIT_EQ(LOW,  r.sum_out,  "FA 1+1+0 sum");
    ASSERT_BIT_EQ(HIGH, r.carry_out,"FA 1+1+0 carry");

    /* 1+1+1 = 1, carry 1 */
    r = fullAdder(HIGH, HIGH, HIGH);
    ASSERT_BIT_EQ(HIGH, r.sum_out,  "FA 1+1+1 sum");
    ASSERT_BIT_EQ(HIGH, r.carry_out,"FA 1+1+1 carry");

    /* 1+0+1 = 0, carry 1 */
    r = fullAdder(HIGH, LOW, HIGH);
    ASSERT_BIT_EQ(LOW,  r.sum_out,  "FA 1+0+1 sum");
    ASSERT_BIT_EQ(HIGH, r.carry_out,"FA 1+0+1 carry");
}

static void test_eight_bit_adder(void) {
    bit carry_out;

    /* 5 + 3 = 8 */
    byte five  = hexToByte("0x05");
    byte three = hexToByte("0x03");
    byte eight = hexToByte("0x08");
    byte result = eightBitAdder(five, three, LOW, &carry_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(eight.bits[i], result.bits[i], "5+3=8");
    ASSERT_BIT_EQ(LOW, carry_out, "5+3 no carry");

    /* 255 + 1 = 0, carry out */
    byte ff  = hexToByte("0xFF");
    byte one = hexToByte("0x01");
    byte zero = hexToByte("0x00");
    result = eightBitAdder(ff, one, LOW, &carry_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(zero.bits[i], result.bits[i], "255+1 overflow");
    ASSERT_BIT_EQ(HIGH, carry_out, "255+1 carry");

    /* 0 + 0 = 0 */
    result = eightBitAdder(zero, zero, LOW, &carry_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(LOW, result.bits[i], "0+0=0");
    ASSERT_BIT_EQ(LOW, carry_out, "0+0 no carry");
}

static void test_eight_bit_subtractor(void) {
    bit borrow_out;

    /* 10 - 3 = 7 */
    byte ten   = hexToByte("0x0A");
    byte three = hexToByte("0x03");
    byte seven = hexToByte("0x07");
    byte result = eightBitSubtractor(ten, three, LOW, &borrow_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(seven.bits[i], result.bits[i], "10-3=7");
    ASSERT_BIT_EQ(LOW, borrow_out, "10-3 no borrow");

    /* 0 - 1 = 255, borrow out */
    byte zero = hexToByte("0x00");
    byte one  = hexToByte("0x01");
    byte ff   = hexToByte("0xFF");
    result = eightBitSubtractor(zero, one, LOW, &borrow_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(ff.bits[i], result.bits[i], "0-1 underflow");
    ASSERT_BIT_EQ(HIGH, borrow_out, "0-1 borrow");

    /* 5 - 5 = 0 */
    byte five = hexToByte("0x05");
    byte expected_zero = hexToByte("0x00");
    result = eightBitSubtractor(five, five, LOW, &borrow_out);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(expected_zero.bits[i], result.bits[i], "5-5=0");
    ASSERT_BIT_EQ(LOW, borrow_out, "5-5 no borrow");
}

int main(void) {
    test_half_adder();
    test_full_adder();
    test_eight_bit_adder();
    test_eight_bit_subtractor();
    PRINT_SUMMARY("test_adders");
    return _tests_failed;
}