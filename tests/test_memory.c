/**
 * @file test_memory.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for the 8-bit latch: write, read, and hold behavior.
 * @version 0.1
 * @date 2026-03-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../logic.h"
#include "test_framework.h"

static void test_latch_write_and_read(void) {
    eight_bit_latch latch = newEightBitLatch();

    /* Write 0b10110010 (178) and read it back */
    byte val = {{1,0,1,1,0,0,1,0}};
    eightBitLatch(&latch, val, HIGH);
    byte out = eightBitLatchOutput(&latch);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(val.bits[i], out.bits[i], "latch write/read 178");
}

static void test_latch_hold(void) {
    eight_bit_latch latch = newEightBitLatch();

    /* Write 42 */
    byte forty_two = {{0,0,1,0,1,0,1,0}};
    eightBitLatch(&latch, forty_two, HIGH);

    /* Present a different value but clock LOW: latch should not update */
    byte ff = {{1,1,1,1,1,1,1,1}};
    eightBitLatch(&latch, ff, LOW);

    byte out = eightBitLatchOutput(&latch);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(forty_two.bits[i], out.bits[i], "latch hold on clock-LOW");
}

static void test_latch_overwrite(void) {
    eight_bit_latch latch = newEightBitLatch();

    byte first  = {{0,0,0,0,1,1,1,1}};  /* 15 */
    byte second = {{1,1,1,1,0,0,0,0}};  /* 240 */

    eightBitLatch(&latch, first,  HIGH);
    eightBitLatch(&latch, second, HIGH);

    byte out = eightBitLatchOutput(&latch);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(second.bits[i], out.bits[i], "latch overwrite");
}

static void test_latch_zero(void) {
    eight_bit_latch latch = newEightBitLatch();

    byte ff   = {{1,1,1,1,1,1,1,1}};
    byte zero = {{0,0,0,0,0,0,0,0}};

    eightBitLatch(&latch, ff,   HIGH);
    eightBitLatch(&latch, zero, HIGH);

    byte out = eightBitLatchOutput(&latch);
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(zero.bits[i], out.bits[i], "latch write zero");
}

int main(void) {
    test_latch_write_and_read();
    test_latch_hold();
    test_latch_overwrite();
    test_latch_zero();
    PRINT_SUMMARY("test_memory");
    return _tests_failed;
}