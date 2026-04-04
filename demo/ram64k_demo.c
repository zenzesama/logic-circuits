/**
 * @file ram64k_demo.c
 * @author zenzesama (zenzesama@gmail.com)
 * @brief demonstrates the 64Kx8 RAM
 * @version 0.1
 * @date 2026-03-28
 *
 * @copyright Copyright (c) 2026
 */

#include <stdio.h>
#include "../include/logic.h"

int main(void){
    printf("64Kx8 RAM Demonstration\n");
    printf("=======================\n\n");

    ram64kx8* ram = newRam64kx8();

    /* test cases: spread across the full 64KB address space */
    struct { char* hi; char* lo; char* data; } tests[] = {
        { "0x00", "0x00", "0xAB" },   /* bottom of memory          */
        { "0x00", "0x01", "0xCD" },   /* one above bottom          */
        { "0x12", "0x34", "0x55" },   /* somewhere in the middle   */
        { "0xDE", "0xAD", "0xBE" },   /* 0xDEAD                    */
        { "0xFF", "0xFE", "0x11" },   /* one below top             */
        { "0xFF", "0xFF", "0xFF" },   /* top of memory             */
    };
    int n = sizeof(tests) / sizeof(tests[0]);

    /* write phase */
    printf("Writing test values:\n");
    printf("Address | Data | Decimal\n");
    printf("--------|------|--------\n");

    for(int i = 0; i < n; i++){
        byte data = hexToByte(tests[i].data);
        writeRam64kx8(ram, tests[i].hi, tests[i].lo, data);

        char data_str[5];
        byteToHex(data, data_str);
        printf("0x%s%s | %s |  %3d\n",
            tests[i].hi + 2,   /* skip "0x" prefix for compact display */
            tests[i].lo + 2,
            data_str,
            byteToDecimal(data)
        );
    }

    /* read phase */
    printf("\nReading back:\n");
    printf("Address | Data | Decimal | Status\n");
    printf("--------|------|---------|-------\n");

    for(int i = 0; i < n; i++){
        byte expected = hexToByte(tests[i].data);
        byte result   = readRam64kx8(ram, tests[i].hi, tests[i].lo);

        char result_str[5];
        byteToHex(result, result_str);
        printf("0x%s%s | %s |  %3d    | %s\n",
            tests[i].hi + 2,
            tests[i].lo + 2,
            result_str,
            byteToDecimal(result),
            byteToDecimal(result) == byteToDecimal(expected) ? "OK" : "MISMATCH"
        );
    }

    return 0;
}