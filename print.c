/**
 * @file print.c
 * @author zenze (zenzesama@gmail.com)
 * @brief this file contaisn the implementation of all print functions
 * @version 0.1
 * @date 2026-02-08
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include "logic.h"

#include <stdio.h>

/**
 * @brief this puts a single bit value on the terminal as a char.
 * 
 * @param a this is the bit that is to be printed, it compares if it is 1 or 0 and prints whatever it is.
 */
void printBit(bit a){
    putchar(a ? '1' : '0');
}

/**
 * @brief this puts a whole byte on the terminal such that the MSB is at left.
 * 
 * @param byte_a the byte is a array of 8 bits, we iterate that array from left to right and print each bit.
 */
void printByte(byte byte_a){
    for (int i = 0; i < 8; i++){
        printBit(byte_a.bits[i]);
    }
    putchar('\n');
}