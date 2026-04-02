/**
 * @file counter16_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the 16-bit program counter
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "logic.h"

int main(void) {
    printf("16-Bit Program Counter Demonstration\n");
    printf("=====================================\n\n");

    sixteen_bit_counter pc = newSixteenBitCounter();
    byte hi, lo;
    char hi_str[5], lo_str[5];

    /* count from 0x0000 to 0x0005 */
    printf("Counting from 0x0000:\n");
    printf("Tick | Address\n");
    printf("-----|--------\n");
    for(int tick = 0; tick <= 5; tick++){
        getSixteenBitCounterValue(&pc, &hi, &lo);
        byteToHex(hi, hi_str);
        byteToHex(lo, lo_str);
        printf("%4d | %s%s\n", tick, hi_str, lo_str + 2);
        tickSixteenBitCounter(&pc);
    }

    /* jump to 0x2000 and count a few more */
    printf("\nJumping to 0x2000:\n");
    printf("Tick | Address\n");
    printf("-----|--------\n");
    jumpSixteenBitCounter(&pc, "0x20", "0x00");
    for(int tick = 0; tick <= 4; tick++){
        getSixteenBitCounterValue(&pc, &hi, &lo);
        byteToHex(hi, hi_str);
        byteToHex(lo, lo_str);
        printf("%4d | %s%s\n", tick, hi_str, lo_str + 2);
        tickSixteenBitCounter(&pc);
    }

    /* demonstrate overflow at 0xFFFF -> 0x0000 */
    printf("\nOverflow at 0xFFFF:\n");
    printf("Tick | Address\n");
    printf("-----|--------\n");
    jumpSixteenBitCounter(&pc, "0xFF", "0xFD");
    for(int tick = 0; tick <= 4; tick++){
        getSixteenBitCounterValue(&pc, &hi, &lo);
        byteToHex(hi, hi_str);
        byteToHex(lo, lo_str);
        printf("%4d | %s%s\n", tick, hi_str, lo_str + 2);
        tickSixteenBitCounter(&pc);
    }

    /* reset */
    printf("\nAfter reset:\n");
    resetSixteenBitCounter(&pc);
    getSixteenBitCounterValue(&pc, &hi, &lo);
    byteToHex(hi, hi_str);
    byteToHex(lo, lo_str);
    printf("     | %s%s\n", hi_str, lo_str + 2);

    return 0;
}