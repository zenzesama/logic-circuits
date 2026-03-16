/**
 * @file test_selector.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for the 1-bit and 8-bit 2-to-1 multiplexers.
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../logic.h"
#include "test_framework.h"

static void test_two_to_one_selector(void) {
    /* Select=0: always return a */
    ASSERT_BIT_EQ(LOW,  twoToOneSelector(LOW,  LOW,  LOW), "MUX sel=0 a=0 b=0");
    ASSERT_BIT_EQ(LOW,  twoToOneSelector(LOW,  HIGH, LOW), "MUX sel=0 a=0 b=1");
    ASSERT_BIT_EQ(HIGH, twoToOneSelector(HIGH, LOW,  LOW), "MUX sel=0 a=1 b=0");
    ASSERT_BIT_EQ(HIGH, twoToOneSelector(HIGH, HIGH, LOW), "MUX sel=0 a=1 b=1");

    /* Select=1: always return b */
    ASSERT_BIT_EQ(LOW,  twoToOneSelector(LOW,  LOW,  HIGH), "MUX sel=1 a=0 b=0");
    ASSERT_BIT_EQ(HIGH, twoToOneSelector(LOW,  HIGH, HIGH), "MUX sel=1 a=0 b=1");
    ASSERT_BIT_EQ(LOW,  twoToOneSelector(HIGH, LOW,  HIGH), "MUX sel=1 a=1 b=0");
    ASSERT_BIT_EQ(HIGH, twoToOneSelector(HIGH, HIGH, HIGH), "MUX sel=1 a=1 b=1");
}

static void test_eight_bit_two_to_one_selector(void) {
    byte a = {{1,0,1,0,1,0,1,0}};  /* 0xAA */
    byte b = {{0,1,0,1,0,1,0,1}};  /* 0x55 */

    /* Select=0: should return a */
    byte result = eightBitTwoToOneSelector(a, b, LOW);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(a.bits[i], result.bits[i], "8-bit MUX sel=0 returns a");

    /* Select=1: should return b */
    result = eightBitTwoToOneSelector(a, b, HIGH);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(b.bits[i], result.bits[i], "8-bit MUX sel=1 returns b");
}

int main(void) {
    test_two_to_one_selector();
    test_eight_bit_two_to_one_selector();
    PRINT_SUMMARY("test_selector");
    return _tests_failed;
}