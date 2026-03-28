/**
 * @file test_ram.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief Unit tests for ram8x8 and ram64kx8
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include "test_framework.h"
#include "logic.h"

/* -------------------------------------------------------------------------
 * helper: assert two bytes are equal bit by bit
 * ------------------------------------------------------------------------- */
static void assert_byte_eq(byte expected, byte actual, const char* label){
    for(int i = 0; i < 8; i++){
        ASSERT_BIT_EQ(expected.bits[i], actual.bits[i], label);
    }
}

/* -------------------------------------------------------------------------
 * ram8x8 — initialization
 * ------------------------------------------------------------------------- */

static void test_ram8x8_initializes_to_zero(void){
    ram8x8 ram = newRam8x8();
    byte zero = hexToByte("0x00");
    char* addrs[8] = {"0x00","0x01","0x02","0x03","0x04","0x05","0x06","0x07"};
    for(int i = 0; i < 8; i++){
        byte result = readRam8x8(&ram, addrs[i]);
        assert_byte_eq(zero, result, "ram8x8 init zero");
    }
}

/* -------------------------------------------------------------------------
 * ram8x8 — basic write then read
 * ------------------------------------------------------------------------- */

static void test_ram8x8_write_read_single(void){
    ram8x8 ram = newRam8x8();
    byte data = hexToByte("0xAB");
    writeRam8x8(&ram, "0x03", data);
    byte result = readRam8x8(&ram, "0x03");
    assert_byte_eq(data, result, "ram8x8 write/read 0x03=0xAB");
}

static void test_ram8x8_write_read_all_addresses(void){
    ram8x8 ram = newRam8x8();
    char* addrs[8]  = {"0x00","0x01","0x02","0x03","0x04","0x05","0x06","0x07"};
    char* values[8] = {"0x01","0x2A","0x55","0x64","0x80","0xAA","0xCC","0xFF"};

    for(int i = 0; i < 8; i++)
        writeRam8x8(&ram, addrs[i], hexToByte(values[i]));

    for(int i = 0; i < 8; i++){
        byte result = readRam8x8(&ram, addrs[i]);
        assert_byte_eq(hexToByte(values[i]), result, "ram8x8 all addresses");
    }
}

/* -------------------------------------------------------------------------
 * ram8x8 — boundary addresses
 * ------------------------------------------------------------------------- */

static void test_ram8x8_boundary_low(void){
    ram8x8 ram = newRam8x8();
    byte data = hexToByte("0xFF");
    writeRam8x8(&ram, "0x00", data);
    byte result = readRam8x8(&ram, "0x00");
    assert_byte_eq(data, result, "ram8x8 boundary 0x00");
}

static void test_ram8x8_boundary_high(void){
    ram8x8 ram = newRam8x8();
    byte data = hexToByte("0xFF");
    writeRam8x8(&ram, "0x07", data);
    byte result = readRam8x8(&ram, "0x07");
    assert_byte_eq(data, result, "ram8x8 boundary 0x07");
}

/* -------------------------------------------------------------------------
 * ram8x8 — overwrite: writing twice keeps the latest value
 * ------------------------------------------------------------------------- */

static void test_ram8x8_overwrite(void){
    ram8x8 ram = newRam8x8();
    writeRam8x8(&ram, "0x02", hexToByte("0xAA"));
    writeRam8x8(&ram, "0x02", hexToByte("0x55"));
    byte result = readRam8x8(&ram, "0x02");
    assert_byte_eq(hexToByte("0x55"), result, "ram8x8 overwrite");
}

/* -------------------------------------------------------------------------
 * ram8x8 — write to one address does not corrupt others
 * ------------------------------------------------------------------------- */

static void test_ram8x8_no_crosstalk(void){
    ram8x8 ram = newRam8x8();
    writeRam8x8(&ram, "0x01", hexToByte("0xAB"));
    writeRam8x8(&ram, "0x02", hexToByte("0xCD"));

    byte r1 = readRam8x8(&ram, "0x01");
    byte r2 = readRam8x8(&ram, "0x02");
    assert_byte_eq(hexToByte("0xAB"), r1, "ram8x8 crosstalk addr 0x01");
    assert_byte_eq(hexToByte("0xCD"), r2, "ram8x8 crosstalk addr 0x02");
}

/* -------------------------------------------------------------------------
 * ram8x8 — read does not modify stored value
 * ------------------------------------------------------------------------- */

static void test_ram8x8_read_is_nondestructive(void){
    ram8x8 ram = newRam8x8();
    byte data = hexToByte("0xBE");
    writeRam8x8(&ram, "0x05", data);
    readRam8x8(&ram, "0x05");   /* first read */
    byte result = readRam8x8(&ram, "0x05");  /* second read */
    assert_byte_eq(data, result, "ram8x8 read nondestructive");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — initialization
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_initializes_to_zero(void){
    ram64kx8* ram = newRam64kx8();
    byte zero = hexToByte("0x00");
    /* spot-check a handful of addresses across the space */
    char* hi[] = {"0x00", "0x00", "0x12", "0xFF", "0xFF"};
    char* lo[] = {"0x00", "0xFF", "0x34", "0x00", "0xFF"};
    for(int i = 0; i < 5; i++){
        byte result = readRam64kx8(ram, hi[i], lo[i]);
        assert_byte_eq(zero, result, "ram64kx8 init zero");
    }
}

/* -------------------------------------------------------------------------
 * ram64kx8 — basic write then read
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_write_read_single(void){
    ram64kx8* ram = newRam64kx8();
    byte data = hexToByte("0xAB");
    writeRam64kx8(ram, "0x12", "0x34", data);
    byte result = readRam64kx8(ram, "0x12", "0x34");
    assert_byte_eq(data, result, "ram64kx8 write/read 0x1234=0xAB");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — boundary addresses
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_boundary_low(void){
    ram64kx8* ram = newRam64kx8();
    byte data = hexToByte("0x01");
    writeRam64kx8(ram, "0x00", "0x00", data);
    byte result = readRam64kx8(ram, "0x00", "0x00");
    assert_byte_eq(data, result, "ram64kx8 boundary 0x0000");
}

static void test_ram64kx8_boundary_high(void){
    ram64kx8* ram = newRam64kx8();
    byte data = hexToByte("0xFF");
    writeRam64kx8(ram, "0xFF", "0xFF", data);
    byte result = readRam64kx8(ram, "0xFF", "0xFF");
    assert_byte_eq(data, result, "ram64kx8 boundary 0xFFFF");
}

static void test_ram64kx8_boundary_mid(void){
    ram64kx8* ram = newRam64kx8();
    byte data = hexToByte("0x55");
    writeRam64kx8(ram, "0x80", "0x00", data);
    byte result = readRam64kx8(ram, "0x80", "0x00");
    assert_byte_eq(data, result, "ram64kx8 boundary 0x8000");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — overwrite keeps latest value
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_overwrite(void){
    ram64kx8* ram = newRam64kx8();
    writeRam64kx8(ram, "0xDE", "0xAD", hexToByte("0x11"));
    writeRam64kx8(ram, "0xDE", "0xAD", hexToByte("0x22"));
    byte result = readRam64kx8(ram, "0xDE", "0xAD");
    assert_byte_eq(hexToByte("0x22"), result, "ram64kx8 overwrite");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — write to one address does not corrupt adjacent addresses
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_no_crosstalk(void){
    ram64kx8* ram = newRam64kx8();
    writeRam64kx8(ram, "0x00", "0x00", hexToByte("0xAA"));
    writeRam64kx8(ram, "0x00", "0x01", hexToByte("0xBB"));
    writeRam64kx8(ram, "0xFF", "0xFF", hexToByte("0xCC"));

    byte r0 = readRam64kx8(ram, "0x00", "0x00");
    byte r1 = readRam64kx8(ram, "0x00", "0x01");
    byte r2 = readRam64kx8(ram, "0xFF", "0xFF");
    assert_byte_eq(hexToByte("0xAA"), r0, "ram64kx8 crosstalk 0x0000");
    assert_byte_eq(hexToByte("0xBB"), r1, "ram64kx8 crosstalk 0x0001");
    assert_byte_eq(hexToByte("0xCC"), r2, "ram64kx8 crosstalk 0xFFFF");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — read is non-destructive
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_read_is_nondestructive(void){
    ram64kx8* ram = newRam64kx8();
    byte data = hexToByte("0xEF");
    writeRam64kx8(ram, "0xAB", "0xCD", data);
    readRam64kx8(ram, "0xAB", "0xCD");
    byte result = readRam64kx8(ram, "0xAB", "0xCD");
    assert_byte_eq(data, result, "ram64kx8 read nondestructive");
}

/* -------------------------------------------------------------------------
 * ram64kx8 — hi/lo byte addressing is independent
 * ------------------------------------------------------------------------- */

static void test_ram64kx8_hi_lo_independence(void){
    ram64kx8* ram = newRam64kx8();
    /* same lo, different hi */
    writeRam64kx8(ram, "0x00", "0xFF", hexToByte("0x11"));
    writeRam64kx8(ram, "0x01", "0xFF", hexToByte("0x22"));
    /* same hi, different lo */
    writeRam64kx8(ram, "0x0A", "0x00", hexToByte("0x33"));
    writeRam64kx8(ram, "0x0A", "0x01", hexToByte("0x44"));

    assert_byte_eq(hexToByte("0x11"), readRam64kx8(ram, "0x00", "0xFF"), "ram64kx8 hi/lo 0x00FF");
    assert_byte_eq(hexToByte("0x22"), readRam64kx8(ram, "0x01", "0xFF"), "ram64kx8 hi/lo 0x01FF");
    assert_byte_eq(hexToByte("0x33"), readRam64kx8(ram, "0x0A", "0x00"), "ram64kx8 hi/lo 0x0A00");
    assert_byte_eq(hexToByte("0x44"), readRam64kx8(ram, "0x0A", "0x01"), "ram64kx8 hi/lo 0x0A01");
}

int main(void){
    /* ram8x8 */
    test_ram8x8_initializes_to_zero();
    test_ram8x8_write_read_single();
    test_ram8x8_write_read_all_addresses();
    test_ram8x8_boundary_low();
    test_ram8x8_boundary_high();
    test_ram8x8_overwrite();
    test_ram8x8_no_crosstalk();
    test_ram8x8_read_is_nondestructive();

    /* ram64kx8 */
    test_ram64kx8_initializes_to_zero();
    test_ram64kx8_write_read_single();
    test_ram64kx8_boundary_low();
    test_ram64kx8_boundary_high();
    test_ram64kx8_boundary_mid();
    test_ram64kx8_overwrite();
    test_ram64kx8_no_crosstalk();
    test_ram64kx8_read_is_nondestructive();
    test_ram64kx8_hi_lo_independence();

    PRINT_SUMMARY("test_ram");
    return _tests_failed;
}