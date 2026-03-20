/**
 * @file test_counter.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for the 8-bit counter: counting, overflow, and reset.
 * @version 0.1
 * @date 2026-03-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "test_framework.h"
#include "logic.h"

/* Helper: compare two bytes bit-by-bit */
static void assert_bytes_eq(byte expected, byte actual, const char* label) {
    for (int i = 0; i < 8; i++)
        ASSERT_BIT_EQ(expected.bits[i], actual.bits[i], label);
}

static void test_counter_init(void) {
    eight_bit_counter counter = newEightBitCounter();
    byte zero = hexToByte("0x00");
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(zero, out, "counter init=0");
}

static void test_counter_single_tick(void) {
    eight_bit_counter counter = newEightBitCounter();
    byte one = hexToByte("0x01");
    
    tickEightBitCounter(&counter);
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(one, out, "counter tick 0->1");
}

static void test_counter_count_to_ten(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    for (int i = 0; i <= 10; i++) {
        byte out = getEightBitCounterValue(&counter);
        /* Create expected value from hex */
        char hex[5];
        hex[0] = '0'; hex[1] = 'x';
        hex[2] = (i < 16) ? '0' : '1';
        hex[3] = "0123456789ABCDEF"[i % 16];
        hex[4] = '\0';
        byte expected = hexToByte(hex);
        assert_bytes_eq(expected, out, "count 0-10");
        if (i < 10) tickEightBitCounter(&counter);
    }
}

static void test_counter_power_of_two(void) {
    /* Test powers of 2: 1, 2, 4, 8, 16, 32, 64, 128 */
    for (int i = 0; i < 8; i++) {
        eight_bit_counter counter = newEightBitCounter();
        int ticks = (1 << i);  /* 2^i */
        for (int j = 0; j < ticks; j++)
            tickEightBitCounter(&counter);
        
        /* Create expected hex value */
        char hex[5] = "0x00";
        int val = (1 << i);
        hex[2] = "0123456789ABCDEF"[val / 16];
        hex[3] = "0123456789ABCDEF"[val % 16];
        byte expected = hexToByte(hex);
        
        byte out = getEightBitCounterValue(&counter);
        assert_bytes_eq(expected, out, "power of 2");
    }
}

static void test_counter_255_overflow(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    byte ff   = hexToByte("0xFF");
    byte zero = hexToByte("0x00");
    
    /* Tick 255 times to reach 255 */
    for (int i = 0; i < 255; i++)
        tickEightBitCounter(&counter);
    
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(ff, out, "counter at 255");
    
    /* One more tick should overflow to 0 */
    tickEightBitCounter(&counter);
    out = getEightBitCounterValue(&counter);
    assert_bytes_eq(zero, out, "counter overflow 255->0");
}

static void test_counter_continue_after_overflow(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    /* Tick 256 times (overflow) then 5 more */
    for (int i = 0; i < 261; i++)
        tickEightBitCounter(&counter);
    
    byte five = hexToByte("0x05");
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(five, out, "counter wraps correctly 261->5");
}

static void test_counter_reset(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    byte hundred = hexToByte("0x64");
    byte zero    = hexToByte("0x00");
    
    /* Count to 100 */
    for (int i = 0; i < 100; i++)
        tickEightBitCounter(&counter);
    
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(hundred, out, "counter at 100");
    
    /* Reset */
    resetEightBitCounter(&counter);
    out = getEightBitCounterValue(&counter);
    assert_bytes_eq(zero, out, "counter reset to 0");
}

static void test_counter_reset_and_count(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    /* Count to 50 */
    for (int i = 0; i < 50; i++)
        tickEightBitCounter(&counter);
    
    /* Reset and count to 7 */
    resetEightBitCounter(&counter);
    for (int i = 0; i < 7; i++)
        tickEightBitCounter(&counter);
    
    byte seven = hexToByte("0x07");
    byte out = getEightBitCounterValue(&counter);
    assert_bytes_eq(seven, out, "reset then count to 7");
}

static void test_counter_binary_patterns(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    /* Test specific binary patterns */
    const char* patterns[5] = {"0x0F", "0x3F", "0x7F", "0xAA", "0xF0"};
    int values[5] = {15, 63, 127, 170, 240};
    
    for (int i = 0; i < 5; i++) {
        resetEightBitCounter(&counter);
        for (int j = 0; j < values[i]; j++)
            tickEightBitCounter(&counter);
        byte expected = hexToByte(patterns[i]);
        byte out = getEightBitCounterValue(&counter);
        assert_bytes_eq(expected, out, "binary pattern");
    }
}

static void test_counter_all_bits_toggle(void) {
    eight_bit_counter counter = newEightBitCounter();
    
    /* After 128 ticks, MSB should be 1 for the first time */
    for (int i = 0; i < 128; i++)
        tickEightBitCounter(&counter);
    
    byte out = getEightBitCounterValue(&counter);
    ASSERT_BIT_EQ(HIGH, out.bits[0], "MSB set at 128");
    
    /* After 128 more (total 256 = overflow), MSB should be 0 again */
    for (int i = 0; i < 128; i++)
        tickEightBitCounter(&counter);
    
    out = getEightBitCounterValue(&counter);
    ASSERT_BIT_EQ(LOW, out.bits[0], "MSB clear after overflow");
}

int main(void) {
    test_counter_init();
    test_counter_single_tick();
    test_counter_count_to_ten();
    test_counter_power_of_two();
    test_counter_255_overflow();
    test_counter_continue_after_overflow();
    test_counter_reset();
    test_counter_reset_and_count();
    test_counter_binary_patterns();
    test_counter_all_bits_toggle();
    PRINT_SUMMARY("test_counter");
    return _tests_failed;
}