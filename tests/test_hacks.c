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

#include "../include/logic.h"
#include "test_framework.h"

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

/* helper: assert byteToHex output matches expected string char by char */
static void assert_hex_str(const char* expected, byte b, const char* label){
    char out[5];
    byteToHex(b, out);
    for(int i = 0; i < 5; i++){
        ASSERT_BIT_EQ(expected[i], out[i], label);
    }
}

static void test_byte_to_hex_zero(void){
    assert_hex_str("0x00", hexToByte("0x00"), "byteToHex 0x00");
}

static void test_byte_to_hex_max(void){
    assert_hex_str("0xFF", hexToByte("0xFF"), "byteToHex 0xFF");
}

static void test_byte_to_hex_low_nibble_only(void){
    assert_hex_str("0x0F", hexToByte("0x0F"), "byteToHex 0x0F");
}

static void test_byte_to_hex_high_nibble_only(void){
    assert_hex_str("0xF0", hexToByte("0xF0"), "byteToHex 0xF0");
}

static void test_byte_to_hex_prefix(void){
    char out[5];
    byteToHex(hexToByte("0x42"), out);
    ASSERT_BIT_EQ('0', out[0], "byteToHex prefix[0] is '0'");
    ASSERT_BIT_EQ('x', out[1], "byteToHex prefix[1] is 'x'");
}

static void test_byte_to_hex_null_terminator(void){
    char out[5];
    byteToHex(hexToByte("0x42"), out);
    ASSERT_BIT_EQ('\0', out[4], "byteToHex null terminator at [4]");
}

static void test_byte_to_hex_body_non_null(void){
    char out[5];
    byteToHex(hexToByte("0x42"), out);
    ASSERT_BIT_EQ(1, out[0] != '\0', "byteToHex out[0] non-null");
    ASSERT_BIT_EQ(1, out[1] != '\0', "byteToHex out[1] non-null");
    ASSERT_BIT_EQ(1, out[2] != '\0', "byteToHex out[2] non-null");
    ASSERT_BIT_EQ(1, out[3] != '\0', "byteToHex out[3] non-null");
}

static void test_byte_to_hex_patterns(void){
    assert_hex_str("0x01", hexToByte("0x01"), "byteToHex 0x01");
    assert_hex_str("0x2A", hexToByte("0x2A"), "byteToHex 0x2A");
    assert_hex_str("0x55", hexToByte("0x55"), "byteToHex 0x55");
    assert_hex_str("0x80", hexToByte("0x80"), "byteToHex 0x80");
    assert_hex_str("0xAA", hexToByte("0xAA"), "byteToHex 0xAA");
    assert_hex_str("0xCC", hexToByte("0xCC"), "byteToHex 0xCC");
    assert_hex_str("0xDE", hexToByte("0xDE"), "byteToHex 0xDE");
    assert_hex_str("0xBE", hexToByte("0xBE"), "byteToHex 0xBE");
}

static void test_byte_to_hex_roundtrip(void){
    static const char digits[] = "0123456789ABCDEF";
    for(int val = 0; val <= 255; val++){
        char hex[5];
        hex[0] = '0';
        hex[1] = 'x';
        hex[2] = digits[val >> 4];
        hex[3] = digits[val & 0xF];
        hex[4] = '\0';

        char out[5];
        byteToHex(hexToByte(hex), out);
        for(int i = 0; i < 5; i++){
            ASSERT_BIT_EQ(hex[i], out[i], "byteToHex roundtrip");
        }
    }
}

int main(void) {
    /* hexToByte */
    test_hex_to_byte_basic();
    test_hex_to_byte_all_ones();
    test_hex_to_byte_pattern();
    test_hex_to_byte_lowercase();

    /* byteToDecimal */
    test_byte_to_decimal_zero();
    test_byte_to_decimal_max();
    test_byte_to_decimal_patterns();
    test_hex_byte_roundtrip();

    /* byteToHex */
    test_byte_to_hex_zero();
    test_byte_to_hex_max();
    test_byte_to_hex_low_nibble_only();
    test_byte_to_hex_high_nibble_only();
    test_byte_to_hex_prefix();
    test_byte_to_hex_null_terminator();
    test_byte_to_hex_body_non_null();
    test_byte_to_hex_patterns();
    test_byte_to_hex_roundtrip();

    PRINT_SUMMARY("test_hacks");
    return _tests_failed;
}