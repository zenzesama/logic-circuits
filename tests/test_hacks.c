/**
 * @file test_hacks.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for hacks utilities
 * @version 0.1
 * @date 2026-03-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "test_framework.h"
#include "logic.h"

static void test_hex_to_byte_basic(void) {
    byte b = hexToByte("0x00");
    ASSERT_BIT_EQ(LOW, b.bits[0], "0x00 bit0");
    ASSERT_BIT_EQ(LOW, b.bits[1], "0x00 bit1");
    ASSERT_BIT_EQ(LOW, b.bits[2], "0x00 bit2");
    ASSERT_BIT_EQ(LOW, b.bits[3], "0x00 bit3");
    ASSERT_BIT_EQ(LOW, b.bits[4], "0x00 bit4");
    ASSERT_BIT_EQ(LOW, b.bits[5], "0x00 bit5");
    ASSERT_BIT_EQ(LOW, b.bits[6], "0x00 bit6");
    ASSERT_BIT_EQ(LOW, b.bits[7], "0x00 bit7");
}

static void test_hex_to_byte_all_ones(void) {
    byte b = hexToByte("0xFF");
    ASSERT_BIT_EQ(HIGH, b.bits[0], "0xFF bit0");
    ASSERT_BIT_EQ(HIGH, b.bits[1], "0xFF bit1");
    ASSERT_BIT_EQ(HIGH, b.bits[2], "0xFF bit2");
    ASSERT_BIT_EQ(HIGH, b.bits[3], "0xFF bit3");
    ASSERT_BIT_EQ(HIGH, b.bits[4], "0xFF bit4");
    ASSERT_BIT_EQ(HIGH, b.bits[5], "0xFF bit5");
    ASSERT_BIT_EQ(HIGH, b.bits[6], "0xFF bit6");
    ASSERT_BIT_EQ(HIGH, b.bits[7], "0xFF bit7");
}

static void test_hex_to_byte_pattern(void) {
    byte b = hexToByte("0xAF");
    ASSERT_BIT_EQ(HIGH, b.bits[0], "0xAF bit0");
    ASSERT_BIT_EQ(LOW,  b.bits[1], "0xAF bit1");
    ASSERT_BIT_EQ(HIGH, b.bits[2], "0xAF bit2");
    ASSERT_BIT_EQ(LOW,  b.bits[3], "0xAF bit3");
    ASSERT_BIT_EQ(HIGH, b.bits[4], "0xAF bit4");
    ASSERT_BIT_EQ(HIGH, b.bits[5], "0xAF bit5");
    ASSERT_BIT_EQ(HIGH, b.bits[6], "0xAF bit6");
    ASSERT_BIT_EQ(HIGH, b.bits[7], "0xAF bit7");
}

static void test_hex_to_byte_lowercase(void) {
    byte b = hexToByte("0x3f");
    ASSERT_BIT_EQ(LOW,  b.bits[0], "0x3f bit0");
    ASSERT_BIT_EQ(LOW,  b.bits[1], "0x3f bit1");
    ASSERT_BIT_EQ(HIGH, b.bits[2], "0x3f bit2");
    ASSERT_BIT_EQ(HIGH, b.bits[3], "0x3f bit3");
    ASSERT_BIT_EQ(HIGH, b.bits[4], "0x3f bit4");
    ASSERT_BIT_EQ(HIGH, b.bits[5], "0x3f bit5");
    ASSERT_BIT_EQ(HIGH, b.bits[6], "0x3f bit6");
    ASSERT_BIT_EQ(HIGH, b.bits[7], "0x3f bit7");
}

static void test_byte_to_decimal_zero(void) {
    byte b = hexToByte("0x00");
    int result = byteToDecimal(b);
    _tests_run++;
    if (result != 0) {
        _tests_failed++;
        printf("  FAIL [byte_to_decimal 0x00]: expected 0, got %d\n", result);
    }
}

static void test_byte_to_decimal_max(void) {
    byte b = hexToByte("0xFF");
    int result = byteToDecimal(b);
    _tests_run++;
    if (result != 255) {
        _tests_failed++;
        printf("  FAIL [byte_to_decimal 0xFF]: expected 255, got %d\n", result);
    }
}

static void test_byte_to_decimal_patterns(void) {
    struct {
        char *hex;
        int expected;
    } tests[] = {
        {"0x01", 1},
        {"0x0A", 10},
        {"0x2A", 42},
        {"0x64", 100},
        {"0x7F", 127},
        {"0x80", 128},
        {"0xAA", 170},
        {"0xC8", 200},
        {"0xF0", 240}
    };
    
    for (int i = 0; i < 9; i++) {
        byte b = hexToByte(tests[i].hex);
        int result = byteToDecimal(b);
        _tests_run++;
        if (result != tests[i].expected) {
            _tests_failed++;
            printf("  FAIL [byte_to_decimal %s]: expected %d, got %d\n", 
                   tests[i].hex, tests[i].expected, result);
        }
    }
}

static void test_hex_byte_roundtrip(void) {
    /* Test that hexToByte and byteToDecimal are inverses */
    for (int val = 0; val <= 255; val++) {
        char hex[5];
        hex[0] = '0'; hex[1] = 'x';
        hex[2] = "0123456789ABCDEF"[val / 16];
        hex[3] = "0123456789ABCDEF"[val % 16];
        hex[4] = '\0';
        
        byte b = hexToByte(hex);
        int result = byteToDecimal(b);
        _tests_run++;
        if (result != val) {
            _tests_failed++;
            printf("  FAIL [roundtrip %s]: expected %d, got %d\n", hex, val, result);
        }
    }
}

int main(void) {
    test_hex_to_byte_basic();
    test_hex_to_byte_all_ones();
    test_hex_to_byte_pattern();
    test_hex_to_byte_lowercase();
    test_byte_to_decimal_zero();
    test_byte_to_decimal_max();
    test_byte_to_decimal_patterns();
    test_hex_byte_roundtrip();
    PRINT_SUMMARY("test_hacks");
    return _tests_failed;
}