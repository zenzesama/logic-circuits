/**
 * @file ram.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief 8x8 RAM built from a 3-to-8 decoder, 8-to-1 selector, and D-latches
 * @version 0.1
 * @date 2026-03-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "../include/logic.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief decodes a 3-bit address into 8 active-HIGH output lines
 * 
 * @param s0 least significant address bit
 * @param s1 middle address bit
 * @param s2 most significant address bit
 * @param out pointer to an 8-element bit array
 * 
 * +-----+-----+-----+------+------+------+------+------+------+------+------+
 * | S2  | S1  | S0  | O7   | O6   | O5   | O4   | O3   |  O2  |  O1  | O0   |
 * +-----+-----+-----+------+------+------+------+------+------+------+------+
 * |  0  |  0  |  0  |  0   |  0   |  0   |  0   |  0   |  0   |  0   | Data |
 * |  0  |  0  |  1  |  0   |  0   |  0   |  0   |  0   |  0   | Data |  0   |
 * |  0  |  1  |  0  |  0   |  0   |  0   |  0   |  0   | Data |  0   |  0   |
 * |  0  |  1  |  1  |  0   |  0   |  0   |  0   | Data |  0   |  0   |  0   |
 * |  1  |  0  |  0  |  0   |  0   |  0   | Data |  0   |  0   |  0   |  0   |
 * |  1  |  0  |  1  |  0   |  0   | Data |  0   |  0   |  0   |  0   |  0   |
 * |  1  |  1  |  0  |  0   | Data |  0   |  0   |  0   |  0   |  0   |  0   |
 * |  1  |  1  |  1  | Data |  0   |  0   |  0   |  0   |  0   |  0   |  0   |
 * +-----+-----+-----+------+------+------+------+------+------+------+------+
 */
void decoder3to8(bit s0, bit s1, bit s2, bit out[8]) {
    bit ns0 = notGate(s0);
    bit ns1 = notGate(s1);
    bit ns2 = notGate(s2);
 
    out[0] = andGate(andGate(ns2, ns1), ns0);
    out[1] = andGate(andGate(ns2, ns1),  s0);
    out[2] = andGate(andGate(ns2,  s1), ns0);
    out[3] = andGate(andGate(ns2,  s1),  s0);
    out[4] = andGate(andGate( s2, ns1), ns0);
    out[5] = andGate(andGate( s2, ns1),  s0);
    out[6] = andGate(andGate( s2,  s1), ns0);
    out[7] = andGate(andGate( s2,  s1),  s0);
}

/**
 * @brief selects one of eight input bits using a 3-bit address
 * 
 * @param in array of 8 input bits; in[0] is selected when address = 0b000
 * @param s0 least significant select bit
 * @param s1 middle select bit
 * @param s2 most significant select bit
 * @return bit the selected input bit
 * 
 * +----+----+----+-----+
 * | S2 | S1 | S0 |  Q  |
 * +----+----+----+-----+
 * | 0  | 0  | 0  | D0  |
 * | 0  | 0  | 1  | D1  |
 * | 0  | 1  | 0  | D2  |
 * | 0  | 1  | 1  | D3  |
 * | 1  | 0  | 0  | D4  |
 * | 1  | 0  | 1  | D5  |
 * | 1  | 1  | 0  | D6  |
 * | 1  | 1  | 1  | D7  |
 * +----+----+----+-----+
 */
bit eightToOneSelector(bit in[8], bit s0, bit s1, bit s2) {
    /* level 0: pair adjacent inputs using s0 */
    bit l0[4];
    l0[0] = twoToOneSelector(in[0], in[1], s0);
    l0[1] = twoToOneSelector(in[2], in[3], s0);
    l0[2] = twoToOneSelector(in[4], in[5], s0);
    l0[3] = twoToOneSelector(in[6], in[7], s0);
 
    bit l1[2];
    l1[0] = twoToOneSelector(l0[0], l0[1], s1);
    l1[1] = twoToOneSelector(l0[2], l0[3], s1);
 
    return twoToOneSelector(l1[0], l1[1], s2);
}

/**
 * @brief initializes a new 8x1 RAM with all latches cleared to LOW
 * 
 * @return ram8x1 the initialized RAM
 */
ram8x1 newRam8x1(void) {
    ram8x1 ram;
    for(int i = 0; i < 8; i++) {
        ram.latches[i] = newDFlipFlop();
    }
    return ram;
}

/**
 * @brief performs a read or write on the 8x1 RAM
 * 
 * @param ram pointer to the ram
 * @param addr 3 bit address packed into bytes, only LSB is used
 * @param data_in the single bit to write
 * @param write HIGH = write data_in into the address
 *              LOW = read data from the latch
 * @return bit that is currently stored at the address location
 */
bit ram8x1Operation(ram8x1* ram, byte addr, bit data_in, bit write) {
    int a = byteToDecimal(addr);
    bit s0 = (a >> 0) & 1;
    bit s1 = (a >> 1) & 1;
    bit s2 = (a >> 2) & 1;

    bit enable[8];
    decoder3to8(s0, s1, s2, enable);

    for(int i = 0; i < 8; i++) {
        bit clk = andGate(enable[i], write);
        dFlipFlop(&ram->latches[i], data_in, clk);
    }

    bit output[8];
    for(int i = 0; i < 8; i++) {
        output[i] = ram->latches[i].rs_ff.q;
    }

    return eightToOneSelector(output, s0, s1, s2);
}

// 8x8 ram here:

/**
 * @brief initializes a new 8x8 RAM with all storage cleared to LOW
 *
 * @return ram8x8 the initialized RAM
 */
ram8x8 newRam8x8(void) {
    ram8x8 ram;
    for(int i = 0; i < 8; i++) {
        ram.lanes[i] = newRam8x1();
    }
    return ram;
}

/**
 * @brief writes a byte into the addressed location
 *
 * @param ram pointer to the RAM
 * @param addr 3-bit address (only the 3 LSBs of the byte are used)
 * @param data byte to write
 *
 * each bit of data is routed to its corresponding bit-lane.
 * all lanes receive the same address and write = HIGH.
 */
void writeRam8x8(ram8x8* ram, char* addr, byte data) {
    byte addr_byte = hexToByte(addr);
    for(int i = 0; i < 8; i++) {
        ram8x1Operation(&ram->lanes[i], addr_byte, data.bits[i], HIGH);
    }
}

/**
 * @brief reads the byte stored at the addressed location
 *
 * @param ram pointer to the RAM
 * @param addr 3-bit address (only the 3 LSBs of the byte are used)
 * @return byte the byte currently stored at that address
 *
 */
byte readRam8x8(ram8x8* ram, char* addr) {
    byte addr_byte = hexToByte(addr);
    byte result;
    bit dummy = LOW;
    for(int i = 0; i < 8; i++) {
        result.bits[i] = ram8x1Operation(&ram->lanes[i], addr_byte, dummy, LOW);
    }
    return result;
}

/* -------------------------------------------------------------------------
 * 64Kx8 RAM  (65536 addresses × 8 bits = 64KB)
 *
 * Hybrid design:
 *   - Storage  : 65536 d_flip_flop structs per bit lane (8 lanes) — genuine
 *                flip-flop storage, architecturally faithful
 *   - Addressing: the 16-bit address is formed from two hex strings (high
 *                 byte + low byte), converted to an integer index via
 *                 byteToDecimal - no 65536-line gate tree, keeps it fast
 *
 * Address format:  addr_hi="0x01", addr_lo="0x00"  →  256
 * ------------------------------------------------------------------------- */

/**
 * @brief initializes a new 64Kx8 RAM with all flip-flops cleared to LOW
 *
 * @return ram64kx8* heap-allocated pointer to the initialized RAM
 *
 * allocated on the heap because 65536 × 8 flip-flops is too large for the
 * stack. caller is responsible for passing this pointer to all operations.
 */
ram64kx8* newRam64kx8(void) {
    ram64kx8* ram = (ram64kx8*)malloc(sizeof(ram64kx8));
    for(int i = 0; i < 65536; i++) {
        for(int b = 0; b < 8; b++) {
            ram->cells[i].bits[b] = newDFlipFlop();
        }
    }
    return ram;
}

/**
 * @brief resolves two hex address strings into a single 16-bit integer index
 *
 * @param addr_hi high byte of the address, e.g. "0x1F"  (bits 15–8)
 * @param addr_lo low  byte of the address, e.g. "0xA0"  (bits  7–0)
 * @return int the resolved address in range 0–65535
 *
 * example: addr_hi="0x01", addr_lo="0x00"  →  256
 */
static int resolveAddress(char* addr_hi, char* addr_lo) {
    int hi = byteToDecimal(hexToByte(addr_hi));
    int lo = byteToDecimal(hexToByte(addr_lo));
    return (hi << 8) | lo;
}

/**
 * @brief writes a byte into the addressed location
 *
 * @param ram pointer to the RAM (from newRam64kx8)
 * @param addr_hi high byte of the 16-bit address, e.g. "0x00"
 * @param addr_lo low  byte of the 16-bit address, e.g. "0xFF"
 * @param data the byte to write
 *
 * resolves the address to an index, then clocks each of the 8 flip-flops
 * in that cell with the corresponding data bit — simulating the write pulse.
 */
void writeRam64kx8(ram64kx8* ram, char* addr_hi, char* addr_lo, byte data) {
    int idx = resolveAddress(addr_hi, addr_lo);
    for(int b = 0; b < 8; b++) {
        dFlipFlop(&ram->cells[idx].bits[b], data.bits[b], HIGH);
        dFlipFlop(&ram->cells[idx].bits[b], data.bits[b], LOW);
    }
}

/**
 * @brief reads the byte stored at the addressed location
 *
 * @param ram     pointer to the RAM
 * @param addr_hi high byte of the 16-bit address, e.g. "0x00"
 * @param addr_lo low  byte of the 16-bit address, e.g. "0xFF"
 * @return byte   the byte currently stored at that address
 *
 * resolves the address to an index, then reads the q output of each
 * flip-flop in that cell to reconstruct the stored byte.
 */
byte readRam64kx8(ram64kx8* ram, char* addr_hi, char* addr_lo) {
    int idx = resolveAddress(addr_hi, addr_lo);
    byte result;
    for(int b = 0; b < 8; b++){
        result.bits[b] = ram->cells[idx].bits[b].rs_ff.q;
    }
    return result;
}